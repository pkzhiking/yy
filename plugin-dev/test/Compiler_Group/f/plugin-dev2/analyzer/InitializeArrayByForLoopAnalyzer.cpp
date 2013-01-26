/*
 * InitializeArrayByForLoopAnalyzer.cpp
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */
#include "include/InitializeArrayByForLoopAnalyzer.h"
#include "../util/include/Logger.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/ASTTree.h"
#include "../util/include/Util.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <iostream>
using std::endl;
extern GTree *globalASTTreePointer;
InitializeArrayByForLoopAnalyzer::InitializeArrayByForLoopAnalyzer()
{
	setAnalyzerNodeType("modify_expr");
}
InitializeArrayByForLoopAnalyzer::~InitializeArrayByForLoopAnalyzer()
{

}
void
InitializeArrayByForLoopAnalyzer::startAnalyze()
{

}
void
InitializeArrayByForLoopAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	//if not in an for loop context, just return
	if(ContextProcessor::getNestedForLoopCount(context)==0)
	{
		return ;
	}
	//under for loop condition
	int forContextNodeIndex = ContextProcessor::getInnterMostForNode(context);
	int inferredLooperNodeIndex = NodeProcessor::inferForLoopLooperVarNodeIndex(globalASTTreePointer->findNodeByIndex(forContextNodeIndex));
	GNode* op0 = NodeProcessor::getOperand(node,0);
	//GNode* op1 = NodeProcessor::getOperand(node,1);
	if (op0->getTreeCode()=="indirect_ref")
	{
		GNode* indirectRefNodeOp0 = NodeProcessor::getOperand(op0,0);
		if(indirectRefNodeOp0->getTreeCode()=="pointer_plus_expr")
		{
			GNode* ptrVarNode = NodeProcessor::getOperand(indirectRefNodeOp0,0);
			GNode* ptrIndexNode = NodeProcessor::getOperand(indirectRefNodeOp0,1);
			if(NodeProcessor::getVarType(ptrVarNode)=="pointer_type" &&
			   NodeProcessor::getVarDeclNodeofNopExprNode(ptrIndexNode)==globalASTTreePointer->findNodeByIndex(inferredLooperNodeIndex))
			{
				Logger::a("InitializeArrayByForLoopAnalyzer")\
				<<"init array in for loop "\
				<<SrcManager::getInstance().getFullFileName()\
				<<":"<<ContextProcessor::inferSrcPositon(context) \
				<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

				stringstream reportMsgStream;
							reportMsgStream<<"InitializeArrayByForLoopAnalyzer: init array in for loop "<<endl;
							string reportMsg = reportMsgStream.str();
							ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
							ProblemList::GetInstance().Add((char*)"InitializeArrayByForLoopAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
			}
		}
	}
}
void
InitializeArrayByForLoopAnalyzer::finishAnalyze()
{

}
void
InitializeArrayByForLoopAnalyzer::clearAnalyzerState()
{

}

