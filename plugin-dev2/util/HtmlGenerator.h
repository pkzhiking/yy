/*
 * HtmlGenerator.h
 *
 *  Created on: Aug 4, 2012
 *      Author: Guo Jiuliang
 */

#ifndef HTMLGENERATOR_H_
#define HTMLGENERATOR_H_
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
using std::vector;
using std::ostream;
using std::map;
using std::string;
using std::stringstream;
# define __analyzer_assert(expr,msg) \
    if (!(expr)) { fprintf(stderr, "%s:%d analyzer assertion failure (msg:%s): %s\n", \
			  __FILE__, __LINE__,msg,# expr); abort(); }

class HtmlGenerator {
public:
	HtmlGenerator();
	void constructHeader();
	void constructStyle();
	void constructBody();
	void constructFooter();
	bool readCode(const string& filename);
	void printContent(ostream& os);
	void setTitle(const string& title);
	void insertReport(int codeLine,const string& reportMsg);
	static void encode(std::string& data);
	virtual ~HtmlGenerator();
private:
	stringstream mContentBuffer;
	string mTitle;
	stringstream mHeaderBuffer;
	stringstream mFooterBuffer;
	vector<string> mCodeLineVector;
	map<int,vector<string> > mReportMap;
};

#endif /* HTMLGENERATOR_H_ */
