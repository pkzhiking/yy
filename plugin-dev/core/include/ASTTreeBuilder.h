/*
 * ASTTreeBuilder.h
 *
 *  Created on: Mar 31, 2012
 *      Author: Guo Jiuliang
 * Description: Helper class to build ASTTree object by using GCC tree walker algorithm
 */
#ifndef AST_TREE_BUILDER_H
#define AST_TREE_BUILDER_H
#include "config.h"
struct ASTTreeBuilder
{
	void (*init)();
	void (*addIndex)(int);
	void (*addProperty)(const char*,const char*);
	void (*addChild)(const char*,int);
	void (*addNode)(int,const char*);
	void (*addULongProperty)(const char*,unsigned long);
	void (*addIntProperty)(const char*,int);
	void (*addRealProperty)(const char*,const char*);
	void (*addFixedProperty)(const char*,const char*);
	void (*addStringProperty)(const char*,const char*);
	void (*addString)(const char*);
	void (*finishFunction)();
	void (*addFunctionName)(const char*);
};
#endif /* AST_TREE_BUILDER_H */



