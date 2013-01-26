/*
 * DivideByTwosExponentanalyzer.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */
#include <iostream>
#include "include/DivideByTwosExponentAnalyzer.h"
#include "../util/include/Util.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Logger.h"
#include "../core/include/ASTTree.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
using std::endl;
DivideByTwosExponentAnalyzer::DivideByTwosExponentAnalyzer()
{
	setAnalyzerNodeType("trunc_div_expr");
}
DivideByTwosExponentAnalyzer::~DivideByTwosExponentAnalyzer()
{

}
void
DivideByTwosExponentAnalyzer::startAnalyze()
{

}
void
DivideByTwosExponentAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	GNode *op1 = NodeProcessor::getOperand(node,1);
	if(NodeProcessor::ifIntConstant(op1))
	{
		int intProp = Util::stringToInt(op1->getProperty("low")->mStringProperty);
		bool value = ifTwosExponent(intProp);
		if(ifTwosExponent(intProp))
		{
			Logger::a("DivideByTwosExponentAnalyzer") \
			<<"divide by 2's exponent value=" \
			<<intProp<<" "<<SrcManager::getInstance().getFullFileName() \
			<<":"<<ContextProcessor::inferSrcPositon(context) \
			<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

			stringstream reportMsgStream;
						reportMsgStream<<"DivideByTwosExponentAnalyzer: divide by 2's exponent value="<<intProp<<endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
						ProblemList::GetInstance().Add((char *)"DivideByTwosExponentAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
		}
	}
}
void
DivideByTwosExponentAnalyzer::finishAnalyze()
{

}
void
DivideByTwosExponentAnalyzer::clearAnalyzerState()
{

}
bool
DivideByTwosExponentAnalyzer::ifTwosExponent(int value)
{
	if(value >0)
	{
		return (value &(value-1)) ==0;
	}
	else
	{
		return ((~value+1) & ~value) ==0;
	}
}
