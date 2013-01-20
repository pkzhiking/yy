/*
 * MultiplyByTwosExponentAnalyzer.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: Guo Jiuliang
 */
#include <iostream>
#include "MultiplyByTwosExponentAnalyzer.h"
#include "Util.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "Logger.h"
#include "ASTTree.h"
#include "SrcManager.h"
using std::endl;
MultiplyByTwosExponentAnalyzer::MultiplyByTwosExponentAnalyzer()
{
	setAnalyzerNodeType("mult_expr");
}
MultiplyByTwosExponentAnalyzer::~MultiplyByTwosExponentAnalyzer()
{

}
void
MultiplyByTwosExponentAnalyzer::startAnalyze()
{

}
void
MultiplyByTwosExponentAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	GNode *op1 = NodeProcessor::getOperand(node,1);
	if(NodeProcessor::ifIntConstant(op1))
	{
		int intProp = Util::stringToInt(op1->getProperty("low")->mStringProperty);
		bool value = ifTwosExponent(intProp);
		if(ifTwosExponent(intProp))
		{
			Logger::a("MultiplyByTwosExponentAnalyzer") \
			<<"multiply by 2's exponent value=" \
			<<intProp<<" "<<SrcManager::getInstance().getFullFileName() \
			<<":"<<ContextProcessor::inferSrcPositon(context) \
			<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
		}
	}
}
void
MultiplyByTwosExponentAnalyzer::finishAnalyze()
{

}
bool
MultiplyByTwosExponentAnalyzer::ifTwosExponent(int value)
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
void
MultiplyByTwosExponentAnalyzer::clearAnalyzerState()
{

}

