/*
 * StringCopyAnalyzer.cpp
 *
 *  Created on: 2012-8-5
 *      Author: dandelion
 */





#include "include/StringCopyAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
StringCopyAnalyzer::StringCopyAnalyzer()
{
	setAnalyzerNodeType("call_expr");
}
StringCopyAnalyzer::~StringCopyAnalyzer()
{

}
void
StringCopyAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
			if (NodeProcessor::isStrCopyFunction(node))
			{
				Logger::a("StringCopyAnalyzer") \
							<<"string copy function strcpy || strncpy found" \
							<<" "<<SrcManager::getInstance().getFullFileName() \
							<<":"<<ContextProcessor::inferSrcPositon(context) <<" " \
							<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

				stringstream reportMsgStream;
								reportMsgStream<<"StringCopyAnalyzer: string copy function strcpy || strncpy found"<<endl;
								string reportMsg = reportMsgStream.str();
								ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
								ProblemList::GetInstance().Add((char*)"StringCopyAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

			}

}
void
StringCopyAnalyzer::startAnalyze()
{

}
void
StringCopyAnalyzer::finishAnalyze()
{

}

void
StringCopyAnalyzer::clearAnalyzerState()
{

}
