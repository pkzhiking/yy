/*
 * ReportManager.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: Guo Jiuliang
 */
#include "include/analyzer-config.h"
#include "include/ReportManager.h"
#include "../util/include/HtmlGenerator.h"
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::ofstream;
using std::fstream;
using std::abort;
using std::string;
using std::make_pair;
ReportManager* ReportManager::mReportManager = NULL;
static string getFilenameFromFullPath(const string& fullpath)
{
	size_t pos = fullpath.rfind('/');
	if(pos==string::npos)
	{
		return fullpath;
	}else
	{
		return fullpath.substr(pos+1);
	}
}
ReportManager::ReportManager() {
	// TODO Auto-generated constructor stub

}

ReportManager::~ReportManager() {
	// TODO Auto-generated destructor stub
}
void
ReportManager::flushReportToFileSystem(const string& dir)
{
	for(map<string,vector<pair<int,string> > >::const_iterator itor=mSrcWithReportsMap.begin();itor!=mSrcWithReportsMap.end();++itor)
	{
		ofstream ofile;
		//ofile.open((dir+getFilenameFromFullPath(itor->first)+".html").c_str());
		ofile.open(((itor->first)+".html").c_str());
		cout << ((itor->first) + ".html").c_str() << endl;
		_analyzer_assert(ofile.is_open(),((dir+getFilenameFromFullPath(itor->first)+".html ")+"create file failed!").c_str());
		HtmlGenerator hg;
		hg.readCode(itor->first);
		for(vector<pair<int,string> >::const_iterator jtor=itor->second.begin();jtor!=itor->second.end();++jtor)
		{
			hg.insertReport(jtor->first,jtor->second);
		}
		hg.constructHeader();
		hg.constructBody();
		hg.constructFooter();
		hg.printContent(ofile);
		ofile.close();
	}
}
void
ReportManager::insertReport(const string& sourceFile,int lineNumber,const string& reportMsg)
{
	mSrcWithReportsMap[sourceFile].push_back(make_pair<int,string>(lineNumber,reportMsg));
}
ReportManager&
ReportManager::getInstance()
{
	if(ReportManager::mReportManager==NULL)
	{
		ReportManager::mReportManager = new ReportManager();
	}
	return *(ReportManager::mReportManager);
}

