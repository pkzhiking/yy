/*
 * ForLoopDivideAnalyzer.cpp
 *
 *  Created on: Aug 9, 2012
 *      Author: kiddlee
 */

#include "include/LoopDivideAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"

#include <iostream>
using std::endl;

LoopDivideAnalyzer::LoopDivideAnalyzer() {
	setAnalyzerNodeType("for_stmt");
	setAnalyzerNodeType("while_stmt");
	setAnalyzerNodeType("do_stmt");
	threshold = 100;
	loopFactor = 2;
}
void
LoopDivideAnalyzer::clearAnalyzerState() {
	threshold = 0;
	loopFactor = 2;
}
LoopDivideAnalyzer::~LoopDivideAnalyzer() {
}

void LoopDivideAnalyzer::analyzeNode(GNode *node, const vector<int> &context) {

	int codeSize = CountCodesInLoop(node);
	if (codeSize > threshold) {
		Logger::a("LoopDivideAnalyzer")
				<< "This loop can be divided into two loops for it has "
				<< codeSize << " code lines"
				<< SrcManager::getInstance().getFullFileName() << ":"
				<< NodeProcessor::getFieldStr(node, "line") << " "
				<< SrcManager::getInstance().getLine(
						Util::stringToInt(
								NodeProcessor::getFieldStr(node, "line")))
				<< endl;

		stringstream reportMsgStream;
							reportMsgStream<<"LoopDivideAnalyzer: This loop can be divided into two loops for it has "
							<< codeSize << " code lines" <<  endl;
							string reportMsg = reportMsgStream.str();
							ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),Util::stringToInt(
									NodeProcessor::getFieldStr(node, "line")),reportMsg);
							ProblemList::GetInstance().Add((char*)"LoopDivideAnalyzer", SrcManager::getInstance().getFullFileName(), Util::stringToInt(NodeProcessor::getFieldStr(node, "line")));

	}
}

void LoopDivideAnalyzer::startAnalyze() {
}

void LoopDivideAnalyzer::finishAnalyze() {
}

int LoopDivideAnalyzer::CountCodesInLoop(GNode * node) {
	int codeSize = 0;
	GNode *loopNode = NodeProcessor::getFieldNode(node, "body");

	if (NodeProcessor::isBindExpr(loopNode)) {
		codeSize += CountCodesInBindExpr(loopNode);
	} else if (NodeProcessor::isCleanupExpr(loopNode)) {
		codeSize += CountCodesInCleanUpPointExpr(loopNode);
	} else if (NodeProcessor::isCondExpr(loopNode)) {
		codeSize += CountCodesInCondExpr(loopNode);
	} else if (NodeProcessor::isLoopStmt(loopNode)) {
		codeSize += LoopDivideAnalyzer::loopFactor * CountCodesInLoop(loopNode);
	}

	return codeSize;
}

int LoopDivideAnalyzer::CountCodesInBindExpr(GNode * initnode) {
	GNode * node = NodeProcessor::getFieldNode(initnode, "body");
	int codeSize = 0;
	if (NodeProcessor::isBindExpr(node)) {
		codeSize += CountCodesInBindExpr(node);
	} else if (NodeProcessor::isStatementList(node)) {
		// iterate the statement list to get all the expressions.
		int index = 0;
		while (true) {
			std::stringstream st;
			st << index;
			GNode * aNode = NodeProcessor::getFieldNode(node, st.str());
			if (NULL == aNode)
				break;
			else {
				if (NodeProcessor::isBindExpr(aNode)) {
					codeSize += CountCodesInBindExpr(aNode);
				} else if (NodeProcessor::isCleanupExpr(aNode)) {
					codeSize += CountCodesInCleanUpPointExpr(aNode);
				} else if (NodeProcessor::isCondExpr(aNode)) {
					codeSize += CountCodesInCondExpr(aNode);
				} else if (NodeProcessor::isLoopStmt(aNode)) {
					codeSize += LoopDivideAnalyzer::loopFactor
							* CountCodesInLoop(aNode);
				}
			}
			index++;
		}
	}
	return codeSize;
}

int LoopDivideAnalyzer::CountCodesInCondExpr(GNode * node) {
	int codeSize = 0;
	int iflines = 0, elselines = 0;
	GNode * ifnode = NodeProcessor::getFieldNode(node, "op 1");
	GNode * elsenode = NodeProcessor::getFieldNode(node, "op 2");

	if (NodeProcessor::isBindExpr(ifnode)) {
		iflines += CountCodesInBindExpr(ifnode);
	} else if (NodeProcessor::isCleanupExpr(ifnode)) {
		iflines += CountCodesInCleanUpPointExpr(ifnode);
	} else if (NodeProcessor::isCondExpr(ifnode)) {
		iflines += CountCodesInCondExpr(ifnode);
	} else if (NodeProcessor::isLoopStmt(ifnode)) {
		iflines += LoopDivideAnalyzer::loopFactor * CountCodesInLoop(ifnode);
	}

	if (NodeProcessor::isBindExpr(elsenode)) {
		elselines += CountCodesInBindExpr(elsenode);
	} else if (NodeProcessor::isCleanupExpr(elsenode)) {
		elselines += CountCodesInCleanUpPointExpr(elsenode);
	} else if (NodeProcessor::isCondExpr(elsenode)) {
		elselines += CountCodesInCondExpr(elsenode);
	} else if (NodeProcessor::isLoopStmt(elsenode)) {
		elselines += CountCodesInLoop(elsenode);
	}

	codeSize += (iflines > elselines) ? iflines : elselines;
	return codeSize;
}

int LoopDivideAnalyzer::CountCodesInCleanUpPointExpr(GNode * node) {
	int codeSize = 1;
	return codeSize;
}
