/*
 * HtmlGenerator.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: Guo Jiuliang
 */
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using std::string;
using std::stringstream;
using std::ifstream;
using std::cout;
using std::endl;
#include "HtmlGenerator.h"
HtmlGenerator::HtmlGenerator() {
	// TODO Auto-generated constructor stub

}
void
HtmlGenerator::constructHeader()
{
	setTitle("检测报告");
	string htmlStart;
	string headerStart;
	string headerEnd;
	mHeaderBuffer<<"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"<<endl;
	mHeaderBuffer<<"<html>"<<endl;
	mHeaderBuffer<<"<head>"<<endl;
	mHeaderBuffer<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"<<endl;
	mHeaderBuffer<<"<title>"+this->mTitle+"</title"<<endl;
	mHeaderBuffer<<"<meta name=\"viewport\" content=\"user-scalable=no, width=device-width height=device-height\" />"<<endl;
	mHeaderBuffer<<"<link rel=\"stylesheet\" type=\"text/css\" href=\"report_style.css\"/>"<<endl;
	mHeaderBuffer<<"</head>"<<endl;
}
void
HtmlGenerator::constructStyle()
{

}
void
HtmlGenerator::constructBody()
{
	mContentBuffer<<"<body>"<<endl;
	mContentBuffer<<"<table class=\"code\">"<<endl;
	mContentBuffer<<"<tbody>"<<endl;
	int lineNumber = 1;
	for(vector<string>::iterator itor = mCodeLineVector.begin();itor!=mCodeLineVector.end();++itor)
	{
		mContentBuffer<<"<tr>"<<endl;
			mContentBuffer<<"<td class=\"num\">"<<endl;
			mContentBuffer<<lineNumber<<endl;
			mContentBuffer<<"</td>"<<endl;
			mContentBuffer<<"<td class=\"line\">"<<endl;
			encode(*itor);
			mContentBuffer<<*itor<<endl;
			if(mReportMap.find(lineNumber)!=mReportMap.end()){
				for(vector<string>::iterator itor=mReportMap[lineNumber].begin();itor!=mReportMap[lineNumber].end();++itor)
				{
					mContentBuffer<<"<tr>"<<endl;
					mContentBuffer<<"<td class=\"num\"></td>";
					mContentBuffer<<"<td class=\"line\">"<<endl;
					mContentBuffer<<"<div class=\"msg msgEvent\" style=\"margin-left:5ex\">"<<endl;
					encode(*itor);
					mContentBuffer<<*itor<<endl;
					mContentBuffer<<"</div>"<<endl;
					mContentBuffer<<"</td>"<<endl;
					mContentBuffer<<"</tr>"<<endl;
				}

			}
			mContentBuffer<<"</td>"<<endl;
		mContentBuffer<<"</tr>"<<endl;
		++lineNumber;
	}
	mContentBuffer<<"</tbody>"<<endl;
	mContentBuffer<<"</table>"<<endl;
	mContentBuffer<<"</body>"<<endl;
}
void
HtmlGenerator::encode(std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':
            buffer.append("&amp;");
            break;
            case '\"':
            buffer.append("&quot;");
            break;
            case '\'':
            buffer.append("&apos;");
            break;
            case '<':
            buffer.append("&lt;");
            break;
            case '>':
            buffer.append("&gt;");
            break;
            default:
            buffer.append(1, data[pos]);
            break;
        }
    }
    data.swap(buffer);
}
void
HtmlGenerator::constructFooter()
{
	mFooterBuffer<<"</html>"<<endl;
}
void
HtmlGenerator::setTitle(const string& title)
{
	this->mTitle = title;
}
bool
HtmlGenerator::readCode(const string& filename)
{
	cout<<"DEBUG:read code file "<<filename<<endl;
	ifstream infile;
	infile.open(filename.c_str(),ifstream::in);
	if(!infile.is_open()){
		return false;
	}
	string codeLine;
	while(getline(infile,codeLine)){
		//cout<<"get line:"<<codeLine<<endl;
		mCodeLineVector.push_back(codeLine);
	}
	return true;
}
void
HtmlGenerator::printContent(ostream& os)
{
	os<<mHeaderBuffer.str()<<endl;
	os<<mContentBuffer.str()<<endl;
	os<<mFooterBuffer.str()<<endl;
}
void
HtmlGenerator::insertReport(int codeLine,const string& reportMsg)
{
	mReportMap[codeLine].push_back(reportMsg);
}
HtmlGenerator::~HtmlGenerator() {
	// TODO Auto-generated destructor stub
}

