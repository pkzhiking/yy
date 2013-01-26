/*
 * FunctionCallNodeProcessor.h
 *
 *  Created on: Apr 23, 2012
 *      Author: Guo Jiuliang
 */

#ifndef FUNCTION_CALL_NODE_PROCESSOR_H
#define FUNCTION_CALL_NODE_PROCESSOR_H
#include "ASTTree.h"
#include <string>
using std::string;
extern GTree* globalASTTreePointer;
class FunctionCallNodeProcessor
{
public:
	static string getFunctionName(GNode* node);
    static string getParameterName(GNode* node);
	static GNode* getParameterNode(GNode* fnNode,int index);
};
#endif /* FUNCTION_CALL_NODE_PROCESSOR_H */
