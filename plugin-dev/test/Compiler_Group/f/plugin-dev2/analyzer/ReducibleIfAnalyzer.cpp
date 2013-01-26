#include "include/ReducibleIfAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/VarNodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

bool LogicUnit::anti(const LogicUnit &x)
{
    if(this->left == x.left && this->right == x.right && NodeProcessor::isLogicAnti(this->ope, x.ope))
	return true;
    return false;
}

ReducibleIfAnalyzer::ReducibleIfAnalyzer()
{
    setAnalyzerNodeType("cond_expr");
}

ReducibleIfAnalyzer::~ReducibleIfAnalyzer()
{
}

void ReducibleIfAnalyzer::startAnalyze()
{
}

void ReducibleIfAnalyzer::finishAnalyze()
{
}

void ReducibleIfAnalyzer::clearAnalyzerState()
{
    vec_LogicUnit.clear();
}

void ReducibleIfAnalyzer::analyzeNode(GNode *node, const vector<int> &context)
{
    GNode *testingNode = NodeProcessor::getOperand(node, 0);
    GNode *ifIfNode = NodeProcessor::getOperand(node, 2);
    GNode *logicLeftNode, *logicRightNode;
    if(NodeProcessor::isLogicAnd(testingNode))
    {
	logicLeftNode = NodeProcessor::getOperand(testingNode, 0);
	logicRightNode = NodeProcessor::getOperand(testingNode, 1);
	if(NodeProcessor::isCompareOperation(logicLeftNode))
	{
	    LogicUnit temp;
	    if(NULL != logicLeftNode)
		temp.ope = logicLeftNode->getTreeCode();
	    GNode *op0Node = NodeProcessor::getOperand(logicLeftNode, 0);
	    GNode *op1Node = NodeProcessor::getOperand(logicLeftNode, 1);
	    temp.left = NodeProcessor::getSimpleOperation(op0Node);
	    temp.right = NodeProcessor::getSimpleOperation(op1Node);
	    vec_LogicUnit.push_back(temp);
	    if(NodeProcessor::isCompareOperation(logicRightNode))
	    {
		if(NULL != logicRightNode)
		    temp.ope = logicRightNode->getTreeCode();
		GNode *op0Node = NodeProcessor::getOperand(logicRightNode, 0);
		GNode *op1Node = NodeProcessor::getOperand(logicRightNode, 1);
		temp.left = NodeProcessor::getSimpleOperation(op0Node);
		temp.right = NodeProcessor::getSimpleOperation(op1Node);
		vec_LogicUnit.push_back(temp);
	    }
	}
    }
    else
    {
	return ;
    }
    if(NodeProcessor::isCondExpr(ifIfNode))
    {
	GNode *ifIfTesting = NodeProcessor::getOperand(ifIfNode, 0);
	if(NodeProcessor::isCompareOperation(ifIfTesting))
	{
	    LogicUnit temp;
	    if(NULL != ifIfTesting)
	    temp.ope = ifIfTesting->getTreeCode();
	    GNode *op0Node = NodeProcessor::getOperand(ifIfTesting, 0);
	    GNode *op1Node = NodeProcessor::getOperand(ifIfTesting, 1);
	    temp.left = NodeProcessor::getSimpleOperation(op0Node);
	    temp.right = NodeProcessor::getSimpleOperation(op1Node);
	    vec_LogicUnit.push_back(temp);
	    GNode *ifIfElseNode = NodeProcessor::getOperand(ifIfNode, 2);
	    if(NodeProcessor::isCondExpr(ifIfElseNode))
	    {
		if(vec_LogicUnit[0].anti(vec_LogicUnit[2]))
		{
		    int lineNum;
		    if(NULL != node)
			lineNum = Util::stringToInt(node->getProperty("line")->mStringProperty);
		    Logger::a("ReducibleAnalyzer") << "if statement is reducible" \
			<< SrcManager::getInstance().getFullFileName() << lineNum \
			<< SrcManager::getInstance().getLine(lineNum) << endl;
		    stringstream reportMsgStream;
		    reportMsgStream << "ReducibleAnalyzer: if statement is reducible" << endl;
		    string reportMsg = reportMsgStream.str();
		    ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(), lineNum, reportMsg);
		}
	    }
	}
    }
}
