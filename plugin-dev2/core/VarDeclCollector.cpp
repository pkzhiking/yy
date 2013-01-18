/*
 * VarDeclCollector.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include <algorithm>
#include "include/VarDeclCollector.h"
#include "include/NodeProcessor.h"
#include "include/SrcManager.h"
#include "include/ContextProcessor.h"
#include "../util/include/Logger.h"
#include "include/StructProcessor.h"
#include "include/ASTTree.h"
#include <iostream>

using std::find;
using std::cout;
using std::endl;
extern GTree* globalASTTreePointer;
VarDeclCollector::VarDeclCollector()
{
	//this->mAnalyzerNodeType = "var_decl";
	this->setAnalyzerNodeType("var_decl");
}
void
VarDeclCollector::analyzeNode(GNode* node,const vector<int>& context)
{
	//cout<<"analyze node:"<<node<<endl;
	if(find(mVarNameList.begin(),mVarNameList.end(),node)==mVarNameList.end())
	{
		if(NodeProcessor::isVarRecordType(node))
		{
			//GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
			StructProcessor::getInstance().addStructNode(node);
		}
		mVarNameList.push_back(node);
		mSrcPositionList.push_back(NodeProcessor::getVarDeclPosition(node));
	}
}
void
VarDeclCollector::startAnalyze()
{

}
void
VarDeclCollector::finishAnalyze()
{
	int srcIndex = 0;
	string tag = "VarDeclCollector";
	for(vector<GNode*>::iterator itor = mVarNameList.begin();itor!=mVarNameList.end();++itor,++srcIndex)
	{
		//cout<<"var name:"<<NodeProcessor::getVarName(*itor)<<endl;
		//cout<<"["<<SrcManager::getInstance().getFullFileName()<<":"<<mSrcPositionList.at(srcIndex)<<"]"<<SrcManager::getInstance().getLine(mSrcPositionList.at(srcIndex))<<endl;
		Logger::i(tag)<<"var name:"<<NodeProcessor::getVarName(*itor)<<" @ "<<"["<<SrcManager::getInstance().getFullFileName()<<":"<<mSrcPositionList.at(srcIndex)<<"]"<<SrcManager::getInstance().getLine(mSrcPositionList.at(srcIndex))<<endl;
	}
#ifdef PLUGIN_DEBUG
	StructProcessor::getInstance().printStructResult();
#endif
}
//const string&
//VarDeclCollector::getAnalyzerNodeType() const
//{
//	return mAnalyzerNodeType;
//}
void
VarDeclCollector::clearAnalyzerState()
{
	this->mSrcPositionList.clear();
	this->mVarNameList.clear();
}
VarDeclCollector::~VarDeclCollector()
{

}
bool
VarDeclCollector::isVarPrimitive(GNode* node)
{
	GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
	return NodeProcessor::isTypePrimitive(typeNode);
}



