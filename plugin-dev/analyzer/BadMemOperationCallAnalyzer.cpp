/*
 * BadMemOperationCallAnalyzer.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */
#include "BadMemOperationCallAnalyzer.h"
#include "NodeProcessor.h"
#include "FunctionCallNodeProcessor.h"
#include "Logger.h"
#include "SrcManager.h"
#include "ContextProcessor.h"
#include "VarNodeProcessor.h"
#include <iostream>
using std::endl;
BadMemOperationCallAnalyzer::BadMemOperationCallAnalyzer()
{
	setAnalyzerNodeType("call_expr");
}
BadMemOperationCallAnalyzer::~BadMemOperationCallAnalyzer()
{

}
void
BadMemOperationCallAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	mNodeArrayToBeAnalyzed.push_back(node);
	mContextArray.push_back(context);
}
void
BadMemOperationCallAnalyzer::startAnalyze()
{

}
void
BadMemOperationCallAnalyzer::finishAnalyze()
{
	vector<GNode*>::iterator nItor = mNodeArrayToBeAnalyzed.begin();
	vector<vector<int> >::iterator cItor = mContextArray.begin();
	for(;nItor!=mNodeArrayToBeAnalyzed.end() && cItor!=mContextArray.end();++nItor,++cItor)
	{
		deferedAnalyze(*nItor,*cItor);
	}
}
void
BadMemOperationCallAnalyzer::deferedAnalyze(GNode* node,const vector<int>& context)
{
	//TODO add analyze code here
	Logger::d("BadMemOperationCallAnalyzer")<<"ana function call "<<NodeProcessor::getFunctionCallName(node)<<endl;
	if(NodeProcessor::getFunctionCallName(node)=="memset")
	{
		GNode* param0 = FunctionCallNodeProcessor::getParameterNode(node,0);
		//cout<<"param 0 of call_expr "<<param0->getTreeCode()<<endl;
		if(isParamPointingToPrimitive(param0))
		{
			Logger::a("BadMemOperationCallAnalyzer")\
					<<"using memset to set value of primitive type "\
					<<SrcManager::getInstance().getFullFileName()\
					<<":"<<ContextProcessor::inferSrcPositon(context) \
					<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
		}
		else if(isParamPointingToPOD(param0))
		{
			Logger::a("BadMemOperationCallAnalyzer")\
						<<"using memset to set value of POD type "\
						<<SrcManager::getInstance().getFullFileName()\
						<<":"<<ContextProcessor::inferSrcPositon(context) \
						<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
		}

	}
}
bool
BadMemOperationCallAnalyzer::isParamPointingToPOD(GNode* node)
{
	if(node->getTreeCode()=="pointer_type")
	{
		GNode* varTypeNode = globalASTTreePointer->findNodeByIndex(
				node->getProperty("ptd")->mNodeProperty);
		if(VarNodeProcessor::isPrimitiveType(varTypeNode) || NodeProcessor::isTypePOD(varTypeNode))
		{
			return true;
		}
		return false;
	}
	else if(node->getTreeCode()=="var_decl")
	{
		if(NodeProcessor::isVarPOD(node))
		{
			return true;
		}
		return false;
	}
	else if(node->getTreeCode()=="nop_expr")
	{
		GNode* op0Node = NodeProcessor::getOperand(node,0);
		if(op0Node->getTreeCode()=="addr_expr")
		{
			GNode* objNode = NodeProcessor::getOperand(op0Node,0);
			if(objNode->getTreeCode()=="var_decl")
			{
				return NodeProcessor::isVarPOD(objNode);
			}
			else if(objNode->getTreeCode()=="component_ref")
			{
				//condition of member of record type
				GNode* fieldNode = NodeProcessor::getOperand(objNode,1);
				GNode* fieldTypeNode = globalASTTreePointer->findNodeByIndex(
						fieldNode->getProperty("type")->mNodeProperty
						);
				return NodeProcessor::isTypePOD(fieldTypeNode);
			}
		}
		else if(op0Node->getTreeCode()=="var_decl")
		{
			GNode* varNode = globalASTTreePointer->findNodeByIndex(
					op0Node->getProperty("type")->mNodeProperty
					);
			if(varNode->getTreeCode()=="pointer_type")
			{
				GNode* pointedNodeType = globalASTTreePointer->findNodeByIndex(
						varNode->getProperty("ptd")->mNodeProperty
						);
				return NodeProcessor::isTypePOD(pointedNodeType);
			}
		}
	}
	return false;
}
bool
BadMemOperationCallAnalyzer::isParamPointingToPrimitive(GNode* node)
{
	if(node->getTreeCode()=="pointer_type")
	{
		GNode* varNode = globalASTTreePointer->findNodeByIndex(
				node->getProperty("ptd")->mNodeProperty);
		if(VarNodeProcessor::isPrimitiveType(varNode))
		{
			return true;
		}
		return false;
	}
	else if(node->getTreeCode()=="var_decl")
	{
		if(VarNodeProcessor::isPrimitiveType(node))
		{
			return true;
		}
		return false;
	}
	else if(node->getTreeCode()=="nop_expr")
	{
		GNode* op0Node = NodeProcessor::getOperand(node,0);
		if(op0Node->getTreeCode()=="addr_expr")
		{
			GNode* objNode = NodeProcessor::getOperand(op0Node,0);
			if(objNode->getTreeCode()=="var_decl")
			{
				return VarNodeProcessor::isPrimitiveType(objNode);
			}
			else if(objNode->getTreeCode()=="component_ref")
			{
				//condition of member of record type
				GNode* fieldNode = NodeProcessor::getOperand(objNode,1);
				GNode* fieldTypeNode = globalASTTreePointer->findNodeByIndex(
						fieldNode->getProperty("type")->mNodeProperty
						);
				return NodeProcessor::isTypePrimitive(fieldTypeNode);
			}
		}
		else if(op0Node->getTreeCode()=="var_decl")
		{
			GNode* varNode = globalASTTreePointer->findNodeByIndex(
					op0Node->getProperty("type")->mNodeProperty
					);
			if(varNode->getTreeCode()=="pointer_type")
			{
				GNode* pointedNodeType = globalASTTreePointer->findNodeByIndex(
						varNode->getProperty("ptd")->mNodeProperty
						);
				return NodeProcessor::isTypePrimitive(pointedNodeType);
			}
		}
	}
	return false;
}
void
BadMemOperationCallAnalyzer::clearAnalyzerState()
{
	mNodeArrayToBeAnalyzed.clear();
	mContextArray.clear();
}





