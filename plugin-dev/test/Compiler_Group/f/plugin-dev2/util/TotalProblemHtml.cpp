#include "./include/TotalProblemHtml.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../core/include/TotalProblem.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::stringstream;
using std::ostream;

TotalProblemHtml::TotalProblemHtml()
{
}

void TotalProblemHtml::ConstructHeader()
{
    SetTitle("overall report");
    cHeadBuffer << "<html>" << endl;
    cHeadBuffer << "<head>" << endl;
    cHeadBuffer << "<title>\n" << cTitle << endl << "</title>" << endl;
    cHeadBuffer << "</head>" << endl;
}

void TotalProblemHtml::ConstructD(string id)
{
    cScriptBuffer << "function $D" << id << "(){" << endl;
    cScriptBuffer << "var d=$('" << id <<"content');" << endl;
    cScriptBuffer << "var h=d.offsetHeight;" << endl;
    cScriptBuffer << "var maxh=300" << endl;
    cScriptBuffer << "function dmove(){" << endl;
    cScriptBuffer << "h+=50;" << endl;
    cScriptBuffer << "if(h>=maxh){" << endl;
    cScriptBuffer << "d.style.height='300px';" << endl;
    cScriptBuffer << "clearInterval(iIntervalId);" << endl;
    cScriptBuffer << "}else{" << endl;
    cScriptBuffer << "d.style.display='block';" << endl;
    cScriptBuffer << "d.style.height=h+'px';}}" << endl;
    cScriptBuffer << "iIntervalId=setInterval(dmove,2);}" << endl;
}

void TotalProblemHtml::ConstructD2(string id)
{
    cScriptBuffer << "function $D2" << id << "(){" << endl;
    cScriptBuffer << "var d=$('" << id <<"content');" << endl;
    cScriptBuffer << "var h=d.offsetHeight;" << endl;
    cScriptBuffer << "var maxh=300" << endl;
    cScriptBuffer << "function dmove(){" << endl;
    cScriptBuffer << "h-=50;" << endl;
    cScriptBuffer << "if(h<=0){" << endl;
    cScriptBuffer << "d.style.display='none';" << endl;
    cScriptBuffer << "clearInterval(iIntervalId);" << endl;
    cScriptBuffer << "}else{" << endl;
    cScriptBuffer << "d.style.height=h+'px';}}" << endl;
    cScriptBuffer << "iIntervalId=setInterval(dmove,2);}" << endl;
}

void TotalProblemHtml::ConstructUse(string id)
{
    cScriptBuffer << "function $use" << id << "(){" << endl;
    cScriptBuffer << "var d=$('" << id << "content');" << endl;
    cScriptBuffer << "var sb=$('" << id << "But');" << endl;
    cScriptBuffer << "if(d.style.display=='none'){" << endl;
    cScriptBuffer << "$D" << id << "();" << endl;
    cScriptBuffer << "sb.innerHTML='+';" << endl;
    cScriptBuffer << "}else{" << endl;
    cScriptBuffer << "$D2" << id << "();" << endl;
    cScriptBuffer << "sb.innerHTML= '-';}}" << endl;
}

void TotalProblemHtml::StartBody()
{
    cBodyBuffer<< " <body>" << endl;
}

void TotalProblemHtml::EndBody()
{
    cBodyBuffer << "</body>" << endl;
}

void TotalProblemHtml::StartScript()
{
    cScriptBuffer << "<script>" << endl;
    cScriptBuffer << "function $(element){" << endl;
    cScriptBuffer << "return element=document.getElementById(element);}" << endl;
}

void TotalProblemHtml::EndScript()
{
    cScriptBuffer << "</script>" << endl;
}

void TotalProblemHtml::ConstructBodyLine(string str, int num)
{
    int pos = str.rfind('/');
    string sub = str.substr(pos + 1, str.size() - pos -1);
    cBodyBuffer << "<a href=\"" << sub << ".html\">" << str << ": " << num << "</a>" << "<br />" << endl;
}

void TotalProblemHtml::StartParagraph(string id)
{
    cBodyBuffer << "<p id=\"" << id << "content\">";
}

void TotalProblemHtml::EndParagraph()
{
    cBodyBuffer << "</p>" << endl;
}

void TotalProblemHtml::Span(string id)
{
    cBodyBuffer << "<span id=\"" << id << "But\" onclick=\"$use" << id << "()\">+</span>" << endl;
}

void TotalProblemHtml::AddBodyHead(string str)
{
    cBodyBuffer << "<h1>" << str << "</h1>" << endl;
}

void TotalProblemHtml::ConstructFooter()
{
    cFooterBuffer << "</html>" << endl;
}

void TotalProblemHtml::Output(ostream& os)
{
    os << cHeadBuffer.str() << endl;
    os << cScriptBuffer.str() << endl;
    os << cBodyBuffer.str() << endl;
    os << cFooterBuffer.str() << endl;
}

TotalProblemHtml::~TotalProblemHtml()
{
}
