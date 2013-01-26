/*
 * ReportManager.h
 *
 *  Created on: Aug 4, 2012
 *      Author: Guo Jiuliang
 */

#ifndef REPORTMANAGER_H_
#define REPORTMANAGER_H_
#include <string>
#include <utility>
#include <map>
#include <vector>
using std::string;
using std::map;
using std::pair;
using std::vector;
/*
 * Manager report  message to specific file with line number
 * At the end of analysis,emit all source files with reports
 * in html format or something else
 */
class ReportManager {
public:
	void insertReport(const string& sourceFile,int lineNumber,const string& reportMsg);
	static ReportManager& getInstance();
	void flushReportToFileSystem(const string& dir);
private:
	ReportManager();
	virtual ~ReportManager();
private:
	map<string,vector<pair<int,string> > > mSrcWithReportsMap;
	static ReportManager *mReportManager ;
};

#endif /* REPORTMANAGER_H_ */
