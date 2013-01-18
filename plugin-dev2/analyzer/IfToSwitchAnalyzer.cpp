/*
 * IfToSwitchAnalyzer.cpp
 *
 *  Created on: 2012-8-7
 *      Author: dandelion
 */


#include "include/IfToSwitchAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/VarNodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"
#include <algorithm>
using std::make_pair;

IfToSwitchAnalyzer::IfToSwitchAnalyzer(){
	setAnalyzerNodeType("cond_expr");
}

IfToSwitchAnalyzer::~IfToSwitchAnalyzer(){
}

void
IfToSwitchAnalyzer::analyzeNode(GNode* node,const vector<int>& context){
	conditionNode conNode;

	conNode.varId = NodeProcessor::getConditionVarID(node);
	if (conNode.varId == -1)
		return ;

	conNode.node = node;
	if (node->getProperty("line") != NULL) {
		conNode.iContextLine = Util::stringToInt(node->getProperty("line")->mStringProperty);
	}else {
		conNode.iContextLine = 0;
	}

	if (context.size() >= 1){
		conNode.father = context[context.size() - 1];
	}else{
		conNode.father = -1;
	}

	mConditionNodeVector.push_back(conNode);
	//printf("%d--%d\n", conNode.node->getIndex(), conNode.iContextLine);
}

void
IfToSwitchAnalyzer::startAnalyze(){
}

void
IfToSwitchAnalyzer::finishAnalyze(){

	if (mConditionNodeVector.size() <= 1){
		return ;
	}

	vector < int > conSet;

	sort(mConditionNodeVector.begin(), mConditionNodeVector.end(), IfToSwitchAnalyzer::getVarIDOrder);

	vector<conditionNode >::iterator nItor = mConditionNodeVector.begin();
	conSet.clear();
	conSet.push_back(0);
	nItor++;
	for(int id = 1;nItor!=mConditionNodeVector.end() ;++nItor, id++)	{
		 if (nItor->varId == (nItor - 1)->varId){
			 conSet.push_back(id);
		 }else{
			 isIfToSwitch(conSet);
			 conSet.clear();
			 conSet.push_back(id);
		 }
	}
	isIfToSwitch(conSet);
}

void
IfToSwitchAnalyzer::clearAnalyzerState(){
	mConditionNodeVector.clear();
	mContextArray.clear();
}

bool IfToSwitchAnalyzer::getVarIDOrder(conditionNode a, conditionNode b){
	return (a.varId < b.varId);
}
void IfToSwitchAnalyzer::isIfToSwitch(const vector<int>& conSet){

	if (conSet.size() == 1)
		return ;

//case 1
	conditionNode pre = mConditionNodeVector[conSet[0]];
	int cnt = 1;
	int lineNum = pre.iContextLine;

	for(int id = 1; id < conSet.size(); id++){
		//printf("%d-%d-%d\n",mConditionNodeVector[conSet[id]].node->getIndex(),mConditionNodeVector[conSet[id]].father, mConditionNodeVector[conSet[id]].iContextLine);
		if (mConditionNodeVector[conSet[id]].father != pre.father){
			if (cnt > 2){
				Logger::a("IfToSwitchAnalyzer")\
									<<"if statement list can be changed to switch "\
									<<SrcManager::getInstance().getFullFileName()\
									<<":"<<lineNum \
									<<SrcManager::getInstance().getLine(lineNum)<<endl;

				stringstream reportMsgStream;
							reportMsgStream<<"IfToSwitchAnalyzer: if statement list can be changed to switch "<<endl;
							string reportMsg = reportMsgStream.str();
							ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNum,reportMsg);
							ProblemList::GetInstance().Add((char*)"IfToSwitchAnalyzer", SrcManager::getInstance().getFullFileName(), lineNum);

			}
			pre = mConditionNodeVector[conSet[id]];
			cnt = 1;
			lineNum = mConditionNodeVector[conSet[id]].iContextLine;
		}else{
				cnt++;
				if (mConditionNodeVector[conSet[id]].iContextLine < lineNum){
					lineNum = mConditionNodeVector[conSet[id]].iContextLine;
				}
		}
	}

	if (cnt > 2){
					Logger::a("IfToSwitchAnalyzer")\
										<<"if statement list can be changed to switch "\
										<<SrcManager::getInstance().getFullFileName()\
										<<":"<<lineNum \
										<<SrcManager::getInstance().getLine(lineNum)<<endl;
					stringstream reportMsgStream;
								reportMsgStream<<"IfToSwitchAnalyzer: if statement list can be changed to switch "<<endl;
								string reportMsg = reportMsgStream.str();
								ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNum,reportMsg);

		}


	//case 2
	pre = mConditionNodeVector[conSet[0]];
	int fa = pre.node->getIndex();
	cnt = 1;
	lineNum = pre.iContextLine;

	for(int id = 1; id < conSet.size(); id++){
			if (mConditionNodeVector[conSet[id]].father == fa){
				fa = mConditionNodeVector[conSet[id]].node->getIndex();
				cnt++;
				if (mConditionNodeVector[conSet[id]].iContextLine < lineNum){
									lineNum = mConditionNodeVector[conSet[id]].iContextLine;
				}
			}else{
				if (cnt > 2){
					Logger::a("IfToSwitchAnalyzer")\
															<<"if statement list can be changed to switch "\
															<<SrcManager::getInstance().getFullFileName()\
															<<":"<<lineNum \
															<<SrcManager::getInstance().getLine(lineNum)<<endl;
					stringstream reportMsgStream;
								reportMsgStream<<"IfToSwitchAnalyzer: if statement list can be changed to switch "<<endl;
								string reportMsg = reportMsgStream.str();
								ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNum,reportMsg);
				}
				pre = mConditionNodeVector[conSet[id]];
				fa = pre.node->getIndex();
				cnt = 1;
				lineNum = mConditionNodeVector[conSet[id]].iContextLine;
			}
	}

	if (cnt > 2){
					Logger::a("IfToSwitchAnalyzer")\
										<<"if statement list can be changed to switch "\
										<<SrcManager::getInstance().getFullFileName()\
										<<":"<<lineNum \
										<<SrcManager::getInstance().getLine(lineNum)<<endl;
					stringstream reportMsgStream;
								reportMsgStream<<"IfToSwitchAnalyzer: if statement list can be changed to switch "<<endl;
								string reportMsg = reportMsgStream.str();
								ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNum,reportMsg);

	}

	return ;
}

