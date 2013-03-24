#include "SrcManager.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
using std::ifstream;
using std::exit;
using std::cerr;
using std::endl;
using std::getline;
extern "C" void register_src_full_filename(char* fullFileName)
{
    SrcManager::getInstance().init(fullFileName);
}
extern "C" const char* get_src_full_filename()
{
    return SrcManager::getInstance().getFileName().c_str();
}
SrcManager* SrcManager::mSrcManager = NULL;
    SrcManager&
SrcManager::getInstance()
{
    if(mSrcManager == NULL)
    {
        mSrcManager = new SrcManager();
    }
    return *mSrcManager;
}
SrcManager::SrcManager()
{

}
SrcManager::~SrcManager()
{

}
    void
SrcManager::init(const string& fullFilename)
{
    if(fullFilename==mFullFilename)
    {
        //do nothing
        return;
    }
    else
    {
        //TODO parse out filename & path
        mFullFilename = fullFilename;
        mInputSrcFile.close();
        mInputSrcFile.open(mFullFilename.c_str());
        if(!mInputSrcFile.is_open())
        {
            cerr<<"Read source file "<<mFullFilename<<" failed, exiting..."<<endl;
            exit(1);
        }
    }
    int sepPos = mFullFilename.rfind('/');
    if(sepPos==-1)
    {
        mFilename = mFullFilename;
        mFilePath = "./";
    }
    else
    {
        mFilename = mFullFilename.substr(sepPos+1);
        mFilePath = mFullFilename.substr(0,sepPos+1);
    }
    mFileContent.clear();
    mFileContent.reserve(4096);
    string line;
    while(getline(mInputSrcFile,line))
    {
        Logger::d("src")<<line<<endl;
        mFileContent.push_back(line);
    }
}
const string&
SrcManager::getFileName() const
{
    return mFilename;
}
const string&
SrcManager::getLine(int line) const
{
    if(line > mFileContent.size() || line<=0)
    {
        // line number to large
        return mEmptyLine;
    }
    else
    {
        //one offset between vector index & file index
        return mFileContent.at(line-1);
    }
}
const string&
SrcManager::getFilePath() const
{
    return mFilePath;
}
const string&
SrcManager::getFullFileName() const
{
    return mFullFilename;
}
    void
SrcManager::destroyInstance()
{
    delete mSrcManager;
}
