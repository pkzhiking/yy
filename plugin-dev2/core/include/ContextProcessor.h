/*
 * ContextProcessor.h
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */

#ifndef CONTEXT_PROCESSOR_H
#define CONTEXT_PROCESSOR_H
#include "ASTTree.h"
#include <vector>
using std::vector;
class ContextProcessor
{
public:
	static int inferSrcPositon(const vector<int>& context);
	static int getInnterMostForNode(const vector<int>& context);
	static int getInnerMostWhileNode(const vector<int>& context);
	static int getNestedForLoopCount(const vector<int>& context);
	static int getNestedWhileLoopCount(const vector<int>& context);
	static int getNestedLoopCount(const vector<int>& context);


	static GNode *getPreStmt( const vector<int> &context, int );
};
#endif /* CONTEXT_PROCESSOR_H */
