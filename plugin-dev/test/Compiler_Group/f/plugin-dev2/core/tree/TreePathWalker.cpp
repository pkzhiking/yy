/*
 * TreePathWalker.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: Guo Jiuliang
 */
#include "../include/TreePathWalker.h"
#include "../include/ASTTree.h"
#include "../../util/include/Logger.h"
#include "../include/AnalyzerManager.h"
#include <vector>
#include <algorithm>
#include <iterator>
using std::vector;
using std::ostream_iterator;
TreePathWalker::TreePathWalker(const GTree* tree):
mTree(tree)
{

}
TreePathWalker::~TreePathWalker()
{

}
void
TreePathWalker::startTreeWalk()
{
	GNode* startNode = mTree->findNodeByIndex(1);
	if(startNode!=NULL) //when process totally empty function
	{
		this->mWalkerQueue.push_back(1);
		this->mExecuteTracePosMap[1] = 0;
	}
	while(this->mWalkerQueue.size()>0)
	{
		this->walkOneStep();
	}
	finishTreeWalk();
}
void
TreePathWalker::walkOneStep()
{
	if (this->mWalkerQueue.size()>0)
	{
		int nodeIndex = this->mWalkerQueue.back();
		this->mWalkerQueue.pop_back();
		this->updateWalkerPathContext(nodeIndex);
		this->processNode(nodeIndex);
		//GNode *node = this->mTree->findNodeByIndex(nodeIndex);

	}
}
bool
TreePathWalker::isTreeCodeValidTrace(const std::string& treeCode)
{
	unsigned int treeCodeSize = treeCode.size();
	if(treeCodeSize>4 && (treeCode.substr(treeCodeSize-4)=="decl"
			|| treeCode.substr(treeCodeSize-4)=="list"
			|| treeCode.substr(treeCodeSize-4)=="expr"
			|| treeCode.substr(treeCodeSize-4)=="stmt"
			|| treeCode.substr(treeCodeSize-4)=="type"
			)
	){
		return true;
	}
	return false;
}
void
TreePathWalker::processNode(int nodeIndex)
{
	string tag = "TreePathWalker";
	Logger::d(tag)<<"processNode "<<nodeIndex<<" with context "<<endl;
	copy(mWalkerPathContext.begin(),mWalkerPathContext.end(),ostream_iterator<int>(Logger::d(tag),"->\n"));
	Logger::d(tag)<<endl;
	AnalyzerManager &manager = AnalyzerManager::getInstance();
	manager.processNode(mTree->findNodeByIndex(nodeIndex),mWalkerPathContext);
	this->mWalkerPathContext.push_back(nodeIndex);
	const vector<int>& childs = this->mTree->findNodeByIndex(nodeIndex)->getChilds();
	for(vector<int>::const_iterator itor = childs.begin();itor!=childs.end();++itor)
	{
		if(find(mWalkerQueue.begin(),mWalkerQueue.end(),*itor)==mWalkerQueue.end() &&
		   find(mWalkerPathContext.begin(),mWalkerPathContext.end(),*itor)==mWalkerPathContext.end()
		)
		{
			//add another layer of check to reduce path
						if (isTreeCodeValidTrace(mTree->findNodeByIndex(*itor)->getTreeCode())){
							mExecuteTracePosMap[*itor] = mWalkerPathContext.size();
							this->mWalkerQueue.push_back(*itor);
						}
		}
	}
}
void
TreePathWalker::updateWalkerPathContext(int nodeIndex)
{
	int traceLength = mExecuteTracePosMap.find(nodeIndex)->second;
	mWalkerPathContext.erase(mWalkerPathContext.begin()+traceLength,mWalkerPathContext.end());
}
void
TreePathWalker::finishTreeWalk()
{
	AnalyzerManager &manager = AnalyzerManager::getInstance();
	manager.analyzeEnd();
    manager.clearAnalyzerState();
}


