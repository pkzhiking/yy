/*
 * InitializeByConstantAnalyzer.cpp
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */
#include "include/AssignToConstantAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/ASTTree.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <iostream>
using std::endl;
AssignToConstantAnalyzer::AssignToConstantAnalyzer()
{
	setAnalyzerNodeType("modify_expr");
}
AssignToConstantAnalyzer::~AssignToConstantAnalyzer()
{
	this->setAnalyzerNodeType("modify_expr");
}
void
AssignToConstantAnalyzer::startAnalyze()
{

}
void
AssignToConstantAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	GNode* op1Node = NodeProcessor::getOperand(node,1);
	if(op1Node!=NULL && op1Node->getTreeCode()=="integer_cst")
	{
		int constValue = Util::stringToInt(op1Node->getProperty("low")->mStringProperty);
		Logger::a("AssignToConstantAnalyzer")\
		<<"Here:Assign to integer constant (value="\
		<<constValue<<")"<<SrcManager::getInstance().getFullFileName()\
		<<":"<<ContextProcessor::inferSrcPositon(context) \
		<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

		stringstream reportMsgStream;
				reportMsgStream<<"AssignToConstantAnalyzer: Here:Assign to integer constant (value="<<constValue<<")"<<endl;
				string reportMsg = reportMsgStream.str();
			ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
			ProblemList::GetInstance().Add("AssignToConstantAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
	}
}
void
AssignToConstantAnalyzer::finishAnalyze()
{

}
void
AssignToConstantAnalyzer::clearAnalyzerState()
{

}



