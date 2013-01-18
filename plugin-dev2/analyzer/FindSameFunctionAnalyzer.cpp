/*
 * FindSameFunctionAnalyzer.cpp
 *
 *  Created on: 2012-10-23
 *      Author: dandelion
 */

#include "include/FindSameFunctionAnalyzer.h"
#include "../core/include/ASTTreeRebuild.h"
#include "../core/include/TreePathWalker.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <string.h>
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::stringstream;

FindSameFunctionAnalyzer* FindSameFunctionAnalyzer::mFindSameFunctionAnalyzer = NULL;

FindSameFunctionAnalyzer::FindSameFunctionAnalyzer() {
	mfunctionHashMap.clear();
}

FindSameFunctionAnalyzer::~FindSameFunctionAnalyzer() {
}

FindSameFunctionAnalyzer&
FindSameFunctionAnalyzer::getInstance()
{
	if(FindSameFunctionAnalyzer::mFindSameFunctionAnalyzer==NULL)
	{
		FindSameFunctionAnalyzer::mFindSameFunctionAnalyzer = new FindSameFunctionAnalyzer();
	}
	return *(FindSameFunctionAnalyzer::mFindSameFunctionAnalyzer);
}

void
FindSameFunctionAnalyzer::globalAnalyze(unsigned int hashCode, string function){
	     if (mfunctionHashMap.find(hashCode) != mfunctionHashMap.end()){
	    	 stringstream reportMsgStream;
	    	 					reportMsgStream<<"FindSameFunctionAnalyzer: " << mfunctionHashMap[hashCode] << "maybe same as the " << function;
	    	 					string reportMsg = reportMsgStream.str();
	    	 					ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),1,reportMsg);
	    	 					ProblemList::GetInstance().Add((char*)"FindSameFunctionAnalyzer", SrcManager::getInstance().getFullFileName(), 1);

	     }else{
	    	 mfunctionHashMap[hashCode] = function;
	     	 	 }
}

