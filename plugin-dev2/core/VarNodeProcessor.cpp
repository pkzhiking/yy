/*
 * VarNodeProcessor.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: Guo Jiuliang
 */
#include "include/VarNodeProcessor.h"
#include "include/ASTTree.h"
#include "include/NodeProcessor.h"
bool
VarNodeProcessor::isConstant(GNode* node)
{
	//TODO impl
	return false;
}
bool
VarNodeProcessor::isPrimitiveType(GNode* node)
{
	if(node->getTreeCode()=="var_decl")
	{
		GNode* typeNode = globalASTTreePointer->findNodeByIndex(
							node->getProperty("type")->mNodeProperty);
		if(		typeNode->getTreeCode()=="integer_type" || \
				typeNode->getTreeCode()=="real_type"    || \
				typeNode->getTreeCode()=="enumeral_type"|| \
				typeNode->getTreeCode()=="pointer_type"   ){
			return true;
		  }
	}
	return false;
}
bool
VarNodeProcessor::isVarPODType(GNode* node)
{
	return NodeProcessor::isVarPOD(node);
}
bool
VarNodeProcessor::isPointer(GNode* node)
{
	if(node->getTreeCode()=="var_decl")
	{
		GNode* typeNode = globalASTTreePointer->findNodeByIndex(
							node->getProperty("type")->mNodeProperty);
		if(typeNode->getTreeCode()=="pointer_type")
		{
			return true;
		}
	}
	return false;
}
int
VarNodeProcessor::getVarSize(GNode* node)
{
	//TODO impl
	return 0;
}
int
VarNodeProcessor::getVarSizeAligned(GNode* node)
{
	//TODO impl
	return 0;
}
bool
VarNodeProcessor::isStruct(GNode* node)
{
	return false;
}
bool
VarNodeProcessor::isPointingToPrimitiveType(GNode* node)
{
	return false;
}



