/*
 * FunctionAsLoopVarAnalyzer.cpp
 *
 *  Created on: 2012-8-10
 *      Author: dandelion
 */
#include "include/FunctionAsLoopVarAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"
#include <iostream>
using std::endl;
#include <sstream>
using std::stringstream;

FunctionAsLoopVarAnalyzer::FunctionAsLoopVarAnalyzer()
{
	setAnalyzerNodeType("for_stmt");
}
FunctionAsLoopVarAnalyzer::~FunctionAsLoopVarAnalyzer()
{

}
void
FunctionAsLoopVarAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
		int varID;
		if ((varID = NodeProcessor::subtreeContainFunction(node, "call_expr", "strlen")) != -1){

				if (!NodeProcessor::isVarChanged(node, varID)){

					int lineNumber = Util::stringToInt(node->getProperty("line")->mStringProperty);

					Logger::a("FunctionAsLoopVarAnalyzer")\
										<<"strlen is used as loop variable "\
										<<SrcManager::getInstance().getFullFileName()\
										<<":"<<lineNumber \
										<<SrcManager::getInstance().getLine(lineNumber)<<endl;

					stringstream reportMsgStream;
										reportMsgStream<<"FunctionAsLoopVarAnalyzer: strlen is used as loop variable ";
										string reportMsg = reportMsgStream.str();
										ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNumber,reportMsg);
										ProblemList::GetInstance().Add((char*)"FunctionAsLoopVarAnalyzer", SrcManager::getInstance().getFullFileName(), lineNumber);
				}

		}

}
void
FunctionAsLoopVarAnalyzer::startAnalyze()
{

}
void
FunctionAsLoopVarAnalyzer::finishAnalyze()
{

}

void
FunctionAsLoopVarAnalyzer::clearAnalyzerState()
{

}


