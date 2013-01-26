/*
 * NodeProcessor.h
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#ifndef NODE_PROCESSOR_H
#define NDOE_PROCESSOR_H
#include <iostream>
#include <string>
#include <vector>
#include "Error.h"
using std::string;
using std::vector;
class GNode;
class NodeProcessor
{
public:
	static string getVarName(GNode*) throw (Error);
	static int getVarDeclPosition(GNode*) throw (Error);
	static GNode* getOperand(GNode*,int) throw (Error);
	static string getFieldStr( GNode *, string ) throw ( Error );
	static GNode *getFieldNode( GNode *, string ) throw ( Error );
	static bool ifIntConstant(GNode*) throw (Error);
	static bool isMemApplyFunction(GNode*) throw (Error);
	static int isInLoop(const vector<int>&)throw (Error);
	static int getNodeType(GNode*) throw (Error);
	static int inferForLoopLooperVarNodeIndex(GNode*) throw (Error);
	static string getVarType(GNode*) throw (Error);
	static string getVarTypeName(GNode*) throw (Error);
	static GNode* getVarDeclNodeofNopExprNode(GNode*) throw (Error);
	static string getFunctionCallName(GNode*) throw (Error);
	static bool isTypePrimitive(GNode*) throw (Error);
	static bool isPointer(GNode* ) throw (Error);
	static bool isVarRecordType(GNode* node) throw (Error);
	static bool isVarPOD(GNode* node) throw (Error);
	static bool isTypePOD(GNode* node) throw (Error);
	static bool isRealConstant( GNode * ) throw ( Error );
	static bool isGlobalScope( GNode * ) throw ( Error );
	static bool isAndOrCondNode( GNode * ) throw ( Error );
	static bool isCompCondNode( GNode * ) throw ( Error );
	static bool isVariable( GNode * ) throw ( Error );
	static bool isCleanupExpr( GNode * ) throw ( Error );
	static bool isConvertExpr( GNode * ) throw ( Error );
	static bool isCompoundExpr( GNode * ) throw ( Error );
	static bool isModifyStmt( GNode * ) throw ( Error );/*modify_expr, preincrement and postincrement*/
	static bool isModifyExpr( GNode * ) throw ( Error );/*simply modify_expr*/
	static bool isExprStmt( GNode * ) throw ( Error ); 
	static bool isForStmt( GNode * ) throw ( Error );
	static bool isTheSameType( GNode *, GNode * ) throw ( Error );
	static string getCalledFunctionName(GNode*) throw (Error);
	static bool isStrCopyFunction(GNode*) throw (Error);
	static int getConditionVarID(GNode* )throw (Error);
	static string getCondTesting(GNode *) throw (Error);
	static string getConstant(GNode *) throw (Error);
	static bool isCompareOperation(GNode *) throw (Error);
	static string getCompareOperation(GNode *) throw (Error);
	static string getSimpleOperation(GNode *) throw (Error);
	static bool isLogicAnd(GNode *) throw (Error);
	static bool isLogicAnti(string, string) throw (Error);
	static int subtreeContainFunction(GNode*, string, string) throw (Error);
	static bool isNamedFunction(GNode*, string) throw (Error);
	static bool isVarChanged(GNode*, int )throw (Error);
	static bool isBindExpr(GNode *) throw (Error);
	static bool isCondExpr(GNode *) throw (Error);
	static bool isStatementList(GNode *) throw (Error);
	static bool isLoopStmt(GNode *) throw (Error);
	static bool isFunctionDecl(GNode *node) throw (Error);
        static bool isParamDecl(GNode *node) throw (Error);
        static GNode *isForContainIf(GNode *node) throw (Error);
        static int getConditonVar(GNode *node, string op) throw (Error);
        static bool isCurrentFunctionDecl(GNode *node) throw(Error);
        static GNode *getFuncTypeNode(GNode *node) throw (Error);
        static bool isFunctionReturnRecordType(GNode *node) throw(Error);
        static bool isFunctionPrmsRecordType(GNode *node) throw(Error);
};
#endif /* NODE_PROCESSOR_H */
