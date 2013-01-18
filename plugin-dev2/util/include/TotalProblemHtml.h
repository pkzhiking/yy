#ifndef TOTALPROBLEMHTML_H
#define TOTALPROBLEMHTML_H

#include <vector>
#include <string>
#include <sstream>
#include "../../core/include/TotalProblem.h"

using std::vector;
using std::string;
using std::stringstream;
using std::ostream;

class TotalProblemHtml
{
public:
    TotalProblemHtml();
    void ConstructHeader();
    void ConstructUse(string);
    void ConstructD(string);
    void ConstructD2(string);
    void StartBody();
    void EndBody();
    void AddBodyHead(string);
    void ConstructFooter();
    void SetTitle(string str){cTitle = str;}
    void StartScript();
    void EndScript();
    void Span(string);
    void ConstructBodyLine(string, int);
    void StartParagraph(string);
    void EndParagraph();
    void Output(ostream&);
    ~TotalProblemHtml();
private:
    stringstream cHeadBuffer;
    stringstream cScriptBuffer;
    stringstream cBodyBuffer;
    stringstream cFooterBuffer;
    string cTitle;
};

#endif
