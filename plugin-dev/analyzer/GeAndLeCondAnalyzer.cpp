/*
 * GeAndLeCondAnalyzer.cpp
 *
 */
#include <iostream>
#include "GeAndLeCondAnalyzer.h"
#include "Util.h"
#include "Logger.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "ASTTree.h"
#include "SrcManager.h"
using std::endl;
using std::cout;
GeAndLeCondAnalyzer::GeAndLeCondAnalyzer()
{
	setAnalyzerNodeType("truth_andif_expr");
}
GeAndLeCondAnalyzer::~GeAndLeCondAnalyzer()
{

}
void
GeAndLeCondAnalyzer::startAnalyze()
{

}
void
GeAndLeCondAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
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
			Logger::a("GeAndLeCondAnalyzer") \
			<<">=min && <= max "<<" "<<SrcManager::getInstance().getFullFileName() \
			<<":"<<ContextProcessor::inferSrcPositon(context) \
			<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
	}
}
void
GeAndLeCondAnalyzer::finishAnalyze()
{

}
