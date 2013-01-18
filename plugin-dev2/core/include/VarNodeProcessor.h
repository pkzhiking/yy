/*
 * VarNodeProcessor.h
 *
 *  Created on: Apr 23, 2012
 *      Author: Guo Jiuliang
 */

#ifndef VAR_NODE_PROCESSOR_Hc
#define VAR_NODE_PROCESSOR_H
#include "ASTTree.h"
extern GTree* globalASTTreePointer;
class VarNodeProcessor
{
public:
	static bool isConstant(GNode* node);
	static bool isPrimitiveType(GNode* node);
	static bool isVarPODType(GNode* node);
	static int getVarSize(GNode* node);
	static int getVarSizeAligned(GNode* node);
	static bool isStruct(GNode* node);
	static bool isPointer(GNode* node);
	static bool isPointingToPrimitiveType(GNode* node);
};
#endif /* VAR_NODE_PROCESSOR_H_c */
