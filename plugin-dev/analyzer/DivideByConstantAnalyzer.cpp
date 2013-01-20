#include <iostream>
#include "DivideByConstantAnalyzer.h"
#include "Util.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "Logger.h"
#include "ASTTree.h"
#include "SrcManager.h"
using std::endl;

DivideByConstantAnalyzer::DivideByConstantAnalyzer()
{
    setAnalyzerNodeType("trunc_div_expr");
}

DivideByConstantAnalyzer::~DivideByConstantAnalyzer()
{
}

void
DivideByConstantAnalyzer::startAnalyze()
{
}

void DivideByConstantAnalyzer::analyzeNode(GNode* node, const vector<int>& context)
{
    GNode *op1 = NodeProcessor::getOperand(node, 1);
    if(NodeProcessor::ifIntConstant(op1))
    {
	int value = Util::stringToInt(op1->getProperty("low")->mStringProperty);
	Logger::a("DivideByConstantAnalyzer") \
	<<"divide by constant" \
	<< value << " " << SrcManager::getInstance().getFullFileName() \
	<<":" << ContextProcessor::inferSrcPositon(context) \
	<< SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context)) << endl;
    }
}

void DivideByConstantAnalyzer::finishAnalyze()
{
}
