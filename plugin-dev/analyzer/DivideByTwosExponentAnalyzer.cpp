/*
 * DivideByTwosExponentanalyzer.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */
#include <iostream>
#include "DivideByTwosExponentAnalyzer.h"
#include "Util.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "Logger.h"
#include "ASTTree.h"
#include "SrcManager.h"
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
