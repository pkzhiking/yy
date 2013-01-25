/*
 * MultiConditionAnalyzer.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: kiddlee
 */

#include "include/MultiConditionAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"

MultiConditionAnalyzer::MultiConditionAnalyzer()
{
    setAnalyzerNodeType("cond_expr");
    threshold = 2;
}

MultiConditionAnalyzer::~MultiConditionAnalyzer()
{

}

void MultiConditionAnalyzer::analyzeNode(GNode * node, const vector < int >&context)
{
    int condition_number = 0;
    GNode *op0 = NodeProcessor::getOperand(node, 0);
    GNode *op1 = NodeProcessor::getOperand(node, 1);
    GNode *op2 = NodeProcessor::getOperand(node, 2);

    if (NodeProcessor::isAndOrCondNode(op0)) {
        condition_number++;
        condition_number += truth_and_or_if_expr(op0);
    }
    if (NodeProcessor::isAndOrCondNode(op1)) {
        condition_number++;
        condition_number += truth_and_or_if_expr(op1);
    }
    if (NodeProcessor::isAndOrCondNode(op2)) {
        condition_number++;
        condition_number += truth_and_or_if_expr(op2);
    }

    if (condition_number >= threshold) {
        Logger::a("GeAndLeCondAnalyzer")
            << " " << SrcManager::getInstance().getFullFileName()
            << ":" << ContextProcessor::inferSrcPositon(context)
            << SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context)) << endl;

        stringstream reportMsgStream;
        reportMsgStream << "MultiConditionAnalyzer : " << endl;
        string reportMsg = reportMsgStream.str();
        ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context), reportMsg);
        ProblemList::GetInstance().Add((char *)"MultiConditionAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
    }
}

void MultiConditionAnalyzer::startAnalyze()
{
}

void MultiConditionAnalyzer::finishAnalyze()
{
}

void MultiConditionAnalyzer::clearAnalyzerState()
{
    threshold = 3;
}

int MultiConditionAnalyzer::truth_and_or_if_expr(GNode * node)
{
    int res = 0;
    GNode *op0 = NodeProcessor::getOperand(node, 0);
    GNode *op1 = NodeProcessor::getOperand(node, 1);

    if (NodeProcessor::isAndOrCondNode(op0)){
        res ++;
        res += truth_and_or_if_expr(op0);
    }
    if (NodeProcessor::isAndOrCondNode(op1)){
        res ++;
        res += truth_and_or_if_expr(op1);
    }

    return res;
}
