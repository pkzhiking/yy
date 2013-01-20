/*
 * MacroFunctionAnalyzer.cpp
 *
 *
 */
#include <iostream>
#include "MacroFunctionAnalyzer.h"
#include "Util.h"
#include "Logger.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "ASTTree.h"
#include "SrcManager.h"
using std::endl;
using std::cout;
MacroFunctionAnalyzer::MacroFunctionAnalyzer()
{
	setAnalyzerNodeType("for_stmt");
    setAnalyzerNodeType("call_expr");
    setAnalyzerNodeType("while_stmt");
    //setAnalyzerNodeType("switch_stmt");
    //setAnalyzerNodeType("cond_expr");
}
MacroFunctionAnalyzer::~MacroFunctionAnalyzer()
{

}
void
MacroFunctionAnalyzer::startAnalyze()
{

}
void
MacroFunctionAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
    //cout<<"###"<<node->getTreeCode()<<endl;


    /*
    GNode *op0 = NodeProcessor::getOperand(node, 0);
    GNode *op1 = NodeProcessor::getOperand(node, 1);

    string treeCode0 = op0->getTreeCode();
    string treeCode1 = op1->getTreeCode();
	if((
                (treeCode0.compare("ge_expr")==0 || treeCode0.compare("gt_expr") == 0) 
             && (treeCode1.compare("le_expr")==0 || treeCode1.compare("lt_expr")==0)
       ) 
        || 
       (
                (treeCode0.compare("le_expr")==0 || treeCode0.compare("lt_expr") == 0) 
             && (treeCode1.compare("ge_expr")==0 || treeCode1.compare("gt_expr") == 0)
       )
    )
	{
			Logger::a("MacroFunctionAnalyzer") \
			<<">=min && <= max "<<" "<<SrcManager::getInstance().getFullFileName() \
			<<":"<<ContextProcessor::inferSrcPositon(context) \
			<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
	}
    */
}
void
MacroFunctionAnalyzer::finishAnalyze()
{

}
