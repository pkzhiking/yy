/*
 * GeAndLeCondAnalyzer.cpp
 *
 */
#include <iostream>
#include "include/GeAndLeCondAnalyzer.h"
#include "../util/include/Util.h"
#include "../util/include/Logger.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ASTTree.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
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

			stringstream reportMsgStream;
								reportMsgStream<<"GeAndLeCondAnalyzer : >=min && <= max "<<endl;
								string reportMsg = reportMsgStream.str();
								ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
								ProblemList::GetInstance().Add((char*)"GeAndLeCondAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

	}
}
void
GeAndLeCondAnalyzer::finishAnalyze()
{

}
