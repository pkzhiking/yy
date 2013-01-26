/*
 * ConditionInLoopAnalyzer.cpp
 *
 *  Created on: 2012-10-16
 *      Author: dandelion
 */


#include "include/ConditionInLoopAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/VarNodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"


ConditionInLoopAnalyzer::ConditionInLoopAnalyzer(){
	setAnalyzerNodeType("for_stmt");
}

ConditionInLoopAnalyzer::~ConditionInLoopAnalyzer(){
}

void
ConditionInLoopAnalyzer::analyzeNode(GNode* node,const vector<int>& context){

	GNode* condNode = NodeProcessor::isForContainIf(node);

	if (condNode == NULL)
		return ;

	int varID1 = NodeProcessor::getConditonVar(condNode, "op 0");

  int varID2 = NodeProcessor::getConditonVar(condNode, "op 1");

  if (varID1 == -1 && varID2 == -1)
    	 return;
  
  int lineNumber;
  if (varID1 != -1){
	  if (!NodeProcessor::isVarChanged(node, varID1)){
	 		 lineNumber = Util::stringToInt(node->getProperty("line")->mStringProperty);
	 	 }else{
	 	  		  return ;
	 	   }
     }

  if (varID2 != -1){
	  if (!NodeProcessor::isVarChanged(node, varID2)){
	 		  lineNumber = Util::stringToInt(node->getProperty("line")->mStringProperty);
	 	 }else{
	 	  		  return ;
	 	  }
     }
  stringstream reportMsgStream;
					reportMsgStream<<"ConditionInLoopAnalyzer: condition can be remove from loop ";
					string reportMsg = reportMsgStream.str();
					ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNumber,reportMsg);
					ProblemList::GetInstance().Add((char*)"ConditionInLoopAnalyzer", SrcManager::getInstance().getFullFileName(), lineNumber);


}

void
ConditionInLoopAnalyzer::startAnalyze(){
}

void
ConditionInLoopAnalyzer::finishAnalyze(){
}



