/*
 * StructPrmsAnalyzer.cpp
 *
 *  Created on: 2012-11-6
 *      Author: dandelion
 */
#include "include/StructPrmsAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"
#include <iostream>
using std::endl;
#include <sstream>
using std::stringstream;

StructPrmsAnalyzer::StructPrmsAnalyzer()
{
	setAnalyzerNodeType("function_decl");
	judged = false;

}
StructPrmsAnalyzer::~StructPrmsAnalyzer()
{

}
void
StructPrmsAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{

	if (!NodeProcessor::isCurrentFunctionDecl(node)){
		return ;
	}
	if (judged){
		return ;
	}
	judged = true;
	GNode* funTypeNode = NodeProcessor::getFuncTypeNode(node);

	if (funTypeNode == NULL){
		return ;
	}
	if (NodeProcessor::isFunctionReturnRecordType(funTypeNode)){
		int lineNumber = 1;
		if (node->getProperty("srcp") != NULL){
			string srcpStr = node->getProperty("srcp")->mStringProperty;
			srcpStr = srcpStr.substr(srcpStr.find(":") + 1);
			lineNumber = Util::stringToInt(srcpStr);
		}
		stringstream reportMsgStream;
											reportMsgStream<<"StructPrmsAnalyzer: return value is record type ";
											string reportMsg = reportMsgStream.str();
											ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNumber,reportMsg);
											ProblemList::GetInstance().Add((char*)"StructPrmsAnalyzer", SrcManager::getInstance().getFullFileName(), lineNumber);

	}
	if (NodeProcessor::isFunctionPrmsRecordType(funTypeNode)){
		int lineNumber = 1;
		if (node->getProperty("srcp") != NULL){
			string srcpStr = node->getProperty("srcp")->mStringProperty;
			srcpStr = srcpStr.substr(srcpStr.find(":") + 1);
			lineNumber = Util::stringToInt(srcpStr);
		}
		stringstream reportMsgStream;
											reportMsgStream<<"StructPrmsAnalyzer: parameter is record type ";
											string reportMsg = reportMsgStream.str();
											ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNumber,reportMsg);
											ProblemList::GetInstance().Add((char*)"StructPrmsAnalyzer", SrcManager::getInstance().getFullFileName(), lineNumber);

	}

}
void
StructPrmsAnalyzer::startAnalyze(){
}
void
StructPrmsAnalyzer::finishAnalyze(){
}

void
StructPrmsAnalyzer::clearAnalyzerState(){
	judged = false;
}


