/*
 * BaseAnalyzerReport.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */

#include "include/BaseAnalyzeReport.h"
#include <string>
#include <sstream>
using std::string;
using std::ostringstream;
BaseAnalyzeReport::BaseAnalyzeReport(const string& tag)
{
	this->mReportTag = tag;
}
const string&
BaseAnalyzeReport::makeFormatedReportMsg()
{
	ostringstream sstr;
	sstr<<"["<<mReportTag<<"]"<<srcContent;
	formatedReprotMsg = sstr.str();
	return formatedReprotMsg;
}
inline string
BaseAnalyzeReport::getCompilationUnitFileName() const
{
    return compilationUnitFileName;
}

inline void
BaseAnalyzeReport::setCompilationUnitFileName(string compilationUnitFileName)
{
    this->compilationUnitFileName = compilationUnitFileName;
}

inline string
BaseAnalyzeReport::getCompilationUnitPath() const
{
    return compilationUnitPath;
}

inline void
BaseAnalyzeReport::setCompilationUnitPath(string compilationUnitPath)
{
    this->compilationUnitPath = compilationUnitPath;
}

inline int
BaseAnalyzeReport::getReportLevel() const
{
    return reportLevel;
}

inline void
BaseAnalyzeReport::setReportLevel(int reportLevel)
{
    this->reportLevel = reportLevel;
}

inline string
BaseAnalyzeReport::getReportMsg() const
{
    return reportMsg;
}

inline void
BaseAnalyzeReport::setReportMsg(string reportMsg)
{
    this->reportMsg = reportMsg;
}

inline int
BaseAnalyzeReport::getSrcLineNumber() const
{
    return srcLineNumber;
}

inline void
BaseAnalyzeReport::setSrcLineNumber(int srcLineNumber)
{
    this->srcLineNumber = srcLineNumber;
}

