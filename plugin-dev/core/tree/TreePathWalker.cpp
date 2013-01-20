/*
 * TreePathWalker.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: Guo Jiuliang
 */
#include "TreePathWalker.h"
#include "ASTTree.h"
#include "Logger.h"
#include "AnalyzerManager.h"
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
			mExecuteTracePosMap[*itor] = mWalkerPathContext.size();
			this->mWalkerQueue.push_back(*itor);
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


