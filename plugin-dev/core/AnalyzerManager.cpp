/*
 * AnalyzerManager.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include "AnalyzerManager.h"
#include "BaseAnalyzer.h"
#include "Logger.h"
#include "ASTTree.h"
using std::vector;
using std::copy;
using std::ostream_iterator;
AnalyzerManager* AnalyzerManager::mAnalyzerManager = NULL;
AnalyzerManager&
AnalyzerManager::getInstance()
{
	if(AnalyzerManager::mAnalyzerManager==NULL)
	{
		AnalyzerManager::mAnalyzerManager = new AnalyzerManager();
	}
	return *(AnalyzerManager::mAnalyzerManager);
}
void
AnalyzerManager::destroyInstance()
{
	delete AnalyzerManager::mAnalyzerManager;
	AnalyzerManager::mAnalyzerManager = NULL;
}
void
AnalyzerManager::addAnalyzer(BaseAnalyzer* analyzer)
{
	if(mAnalyzersMap.find(analyzer->getAnalyzerNodeType())==mAnalyzersMap.end())
	{
		mAnalyzersMap[analyzer->getAnalyzerNodeType()] = vector<BaseAnalyzer*>();
	}
	mAnalyzersMap[analyzer->getAnalyzerNodeType()].push_back(analyzer);
	mAnalyzers.push_back(analyzer);
}
void
AnalyzerManager::processNode(GNode* node,const vector<int>& context)
{
	string tag = "AnalyzerManager";
	Logger::d(tag)<<"analysis node "<<node->getIndex()<<endl;
	copy(context.begin(),context.end(),ostream_iterator<int>(Logger::d(tag),"->\n"));
	if(mAnalyzersMap.find(node->getTreeCode())!=mAnalyzersMap.end())
	{
		//cout<<node->getTreeCode()<<endl;
		vector<BaseAnalyzer*> &analyzerVector = mAnalyzersMap[node->getTreeCode()];
		for(vector<BaseAnalyzer*>::iterator itor = analyzerVector.begin();itor!=analyzerVector.end();++itor)
		{
			(*itor)->analyzeNode(node,context);
		}
	}
}
void
AnalyzerManager::analyzeStart()
{
	for(vector<BaseAnalyzer*>::iterator itor = mAnalyzers.begin();itor!=mAnalyzers.end();++itor)
	{
		(*itor)->startAnalyze();
	}
}
void
AnalyzerManager::analyzeEnd()
{
	for(vector<BaseAnalyzer*>::iterator itor = mAnalyzers.begin();itor!=mAnalyzers.end();++itor)
	{
		(*itor)->finishAnalyze();
	}
}
void
AnalyzerManager::clearAnalyzerState()
{
	for(vector<BaseAnalyzer*>::iterator itor = mAnalyzers.begin();itor!=mAnalyzers.end();++itor)
	{
		(*itor)->clearAnalyzerState();
	}
}
AnalyzerManager::~AnalyzerManager()
{
	for(vector<BaseAnalyzer*>::iterator itor = mAnalyzers.begin();itor!=mAnalyzers.end();++itor)
	{
		delete (*itor);
		*itor = NULL;
	}
}
AnalyzerManager::AnalyzerManager()
{}


