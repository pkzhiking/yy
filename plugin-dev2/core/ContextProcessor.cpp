/*
 * ContextProcessor.cpp
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */
#include <sstream>
#include "include/ContextProcessor.h"
#include "include/ASTTree.h"
using std::stringstream;
extern GTree *globalASTTreePointer;
/**
 * return -1 when no position found
 */
int
ContextProcessor::inferSrcPositon(const vector<int>& context)
{
	vector<int>::const_reverse_iterator itor = context.rbegin();
	while(itor!=context.rend())
	{
		const GNode* cur = globalASTTreePointer->findNodeByIndex(*itor);
		if(cur->getTreeCode().rfind("_stmt")==cur->getTreeCode().length()-5)
		{
			stringstream sstr;
			sstr<<cur->getProperty("line")->mStringProperty;
			int lineNumber;
			sstr>>lineNumber;
			return lineNumber;
		}
		else if(cur->getTreeCode()=="cond_expr")
		{
			stringstream sstr;
			//todo: 段错误
			if (cur->getProperty("line") != NULL) {
				sstr<<cur->getProperty("line")->mStringProperty;
				int lineNumber;
				sstr>>lineNumber;
				return lineNumber;
			}else{
				return 0;
			}

		}
		++itor;
	}
	return -1;
}
int
ContextProcessor::getInnterMostForNode(const vector<int>& context)
{
	for(vector<int>::const_reverse_iterator itor = context.rbegin();itor!=context.rend();++itor)
	{
		if (globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="for_stmt")
		{
			return *itor;
		}
	}
	return -1;
}
int
ContextProcessor::getInnerMostWhileNode(const vector<int>& context)
{
	for(vector<int>::const_reverse_iterator itor = context.rbegin();itor!=context.rend();++itor)
	{
		if (globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="while_stmt")
		{
			return *itor;
		}
	}
	return -1;
}
int
ContextProcessor::getNestedForLoopCount(const vector<int>& context)
{
	int count = 0;
	for(vector<int>::const_iterator itor = context.begin();itor<context.end();++itor)
	{
		if (globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="for_stmt")
		{
			++count;
		}
	}
	return count;
}
int
ContextProcessor::getNestedWhileLoopCount(const vector<int>& context)
{
	int count = 0;
	for(vector<int>::const_iterator itor = context.begin();itor<context.end();++itor)
	{
		if (globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="while_stmt")
		{
			++count;
		}
	}
	return count;
}
int
ContextProcessor::getNestedLoopCount(const vector<int>& context)
{
	int count = 0;
	for(vector<int>::const_iterator itor = context.begin();itor<context.end();++itor)
	{
		if (globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="while_stmt" ||
				globalASTTreePointer->findNodeByIndex(*itor)->getTreeCode()=="for_stmt")
		{
			++count;
		}
	}
	return count;
}
GNode *
ContextProcessor::getPreStmt( const vector<int> &context, int currentIndex ){
    /**
     * if context if empty, no precious statement found. 
     */
    if(context.size()==0){
        return NULL;
    }
	GNode *stmtList = globalASTTreePointer->findNodeByIndex( *( context.rbegin() ) );
	if ( "statement_list" != stmtList->getTreeCode() )
		return NULL;
	const vector< int > &childs = stmtList->getChilds();
	int result = 0;
	for(vector<int>::const_iterator itor = childs.begin();itor!=childs.end();++itor)
	{
		if ( *itor == currentIndex ){
			if ( 0 == result )
				return NULL;
			return globalASTTreePointer->findNodeByIndex( result );
		}
		result = *itor;
	}
	return NULL;
}

