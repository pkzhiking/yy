/*
 * FunctionCallNodeProcessor.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: Guo Jiuliang
 */

#include "include/FunctionCallNodeProcessor.h"
#include "../util/include/Util.h"
string
FunctionCallNodeProcessor::getFunctionName(GNode* node)
{
	GNode* fnNameNode = globalASTTreePointer->findNodeByIndex(node->getProperty("name")->mNodeProperty);
	if(fnNameNode->getTreeCode()=="identifier_node")
	{
		if(fnNameNode->getProperty("strg")!=NULL)
		{
			return fnNameNode->getProperty("strg")->mStringProperty;
		}
	}
	return "";
}

string FunctionCallNodeProcessor::getParameterName(GNode *node){
    GNode* paNameNode = globalASTTreePointer->findNodeByIndex(node->getProperty("name")->mNodeProperty);
    if(paNameNode->getTreeCode()=="identifier_node")
    {
        if(paNameNode->getProperty("strg")!=NULL){
            return paNameNode->getProperty("strg")->mStringProperty;
        }
    }
    return "";
}

GNode*
FunctionCallNodeProcessor::getParameterNode(GNode* fnNode,int index)
{
	return globalASTTreePointer->findNodeByIndex(fnNode->getProperty(Util::intToString(index))->mNodeProperty);
}


