#ifndef BASE_ANALYZE_REPORT_H
#define BASE_ANALYZE_REPORT_H
#include <string>
using std::string;
class BaseAnalyzeReport
{
public:
	BaseAnalyzeReport(const string& tag);
	string getCompilationUnitFileName() const;
	void setCompilationUnitFileName(string compilationUnitFileName);
	string getCompilationUnitPath() const;
	void setCompilationUnitPath(string compilationUnitPath);
	int getReportLevel() const;
	void setReportLevel(int reportLevel);
	string getReportMsg() const;
	void setReportMsg(string reportMsg);
	int getSrcLineNumber() const;
	void setSrcLineNumber(int srcLineNumber);
	//interface methods
	virtual const string& makeFormatedReportMsg();
private:
	//about src
	string srcContent;
	string compilationUnitFileName;
	string compilationUnitPath;
	int srcLineNumber;
	//about report
	string reportMsg;
	int reportLevel;
	string formatedReprotMsg;
	string mReportTag;
};
#endif /* BASE_ANALYZE_REPORT_H */
