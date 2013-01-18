/*
 * SrcManager.h
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#ifndef SRC_MANAGER_H
#define SRC_MANAGER_H
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::string;
using std::ifstream;
class SrcManager
{
public:
	void init(const string&);
	static SrcManager& getInstance();
    const string& getLine(int) const;
    const string& getFileName() const;
    const string& getFilePath() const;
    const string& getFullFileName() const;
	void destroyInstance();
private:
	SrcManager();
	~SrcManager();
	SrcManager(SrcManager&){}
	SrcManager& operator=(SrcManager&){return *this;}
	//void destroyInstance();
private:
	string mFilename;
	string mFilePath;
	string mFullFilename;
	string mEmptyLine;
	int mLineNumber;
	ifstream mInputSrcFile;
	vector<string> mFileContent;
	static SrcManager* mSrcManager;
};
#endif /* SRC_MANAGER_H */
