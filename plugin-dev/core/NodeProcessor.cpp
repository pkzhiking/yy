/*
 * NodeProcessor.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include "NodeProcessor.h"
#include "ASTTree.h"
#include "FunctionCallNodeProcessor.h"
#include "StructProcessor.h"
#include "Util.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
using std::endl;
using std::cerr;
using std::string;
using std::stringstream;
using std::boolalpha;
extern GTree *globalASTTreePointer;
string
NodeProcessor::getVarName(GNode* node) throw (Error)
{
	if(node->getTreeCode().substr(node->getTreeCode().length()-4) == "decl")
	{
		const GProperty* name_property = node->getProperty("name");
		if(NULL != name_property)
		{
		    int nameNode = name_property->mNodeProperty;
		    const GProperty *prop = node->_getTree()->findNodeByIndex(nameNode)->getProperty("strg");
		    if(NULL != prop)
		    {
			return prop->mStringProperty;
		    }
		    else
		    {
			return "null";
		    }
		}
		else
		{
		    return "null";
		}
	}
	else
	{
	    throw Error();
	}
}

int
NodeProcessor::getVarDeclPosition(GNode* node) throw (Error)
{
	if(node->getTreeCode()=="var_decl")
	{
		string srcpString = node->getProperty("srcp")->mStringProperty;
		string lineNumberString = srcpString.substr(srcpString.find(':')+1);
		stringstream sstr;
		sstr<<lineNumberString;
		int lineNumber;
		sstr>>lineNumber;
		return lineNumber;
	}
	else
	{
		throw Error();
	}
}
GNode*
NodeProcessor::getOperand(GNode* node,int opIndex) throw (Error)
{
	return globalASTTreePointer->findNodeByIndex(node->getProperty(string("op ")+Util::intToString(opIndex))->mNodeProperty);
}

string NodeProcessor::getFieldStr( GNode *node, string fieldName ) throw ( Error ){
	return node->getProperty( fieldName )->toString();
}

GNode *NodeProcessor::getFieldNode( GNode *node, string fieldName ) throw ( Error ){
	return globalASTTreePointer->findNodeByIndex( node->getProperty( fieldName )->mNodeProperty );
}

bool
NodeProcessor::ifIntConstant(GNode* node) throw (Error)
{
	if(node->getTreeCode()=="integer_cst")
	{
		return true;
	}
	else{
		return false;
	}
}

bool
NodeProcessor::isMemApplyFunction(GNode* node) throw (Error)
{
	GNode* functionDecl = globalASTTreePointer->findNodeByIndex(
			globalASTTreePointer->findNodeByIndex(node->getChilds()[1])->getChilds()[1]);

	GNode* functionName = globalASTTreePointer->findNodeByIndex(functionDecl->getProperty("name")->mNodeProperty);
	string sName = functionName->getProperty("strg")->mStringProperty;
	if (sName == "malloc" || sName == "calloc" || sName == "realloc")
	{
		return true;
	}
	return false;
}
int
NodeProcessor::isInLoop(const vector<int>& context) throw (Error)
{
	int loopCounter = 0;
	GNode* node;
	for(int index = 0; index < context.size(); index++)
	{
		node = globalASTTreePointer->findNodeByIndex(context[index]);
		if (node->getTreeCode() == "for_stmt" || node->getTreeCode() == "while_stmt"){
			loopCounter++;
		}
	}
	return loopCounter;
}
int
NodeProcessor::getNodeType(GNode* node) throw (Error)
{
	string nodeType = node->getTreeCode();
	if (nodeType == "integer_type" || nodeType == "real_type" || nodeType == "boolean_type" || nodeType == "pointer_type")
	{
		return 1;
	}
	if (nodeType == "array_type")
	{
		return 2;
	}
	if (nodeType == "record_type")
	{
		return 3;
	}
	if (nodeType == "union_type")
	{
		return 4;
	}
	cerr << "undefine type ------" << nodeType << endl;
	return 0;
}
string
NodeProcessor::getVarType(GNode* node) throw (Error)
{
	int typeNodeIndex = node->getProperty("type")->mNodeProperty;
	return globalASTTreePointer->findNodeByIndex(typeNodeIndex)->getTreeCode();
}
GNode*
NodeProcessor::getVarDeclNodeofNopExprNode(GNode* node) throw (Error)
{
	if (node->getTreeCode()=="nop_expr")
	{
		GNode* tmpNode = NodeProcessor::getOperand(node,0);
		while(tmpNode->getTreeCode()!="var_decl")
		{
			tmpNode = NodeProcessor::getOperand(tmpNode,0);
		}
		return tmpNode;
	}
	throw Error();
}
int
NodeProcessor::inferForLoopLooperVarNodeIndex(GNode* node) throw (Error)
{
	int result = -1;
	if(node->getTreeCode() == "for_stmt")
	{
		int condNodeIndex = node->getProperty("cond")->mNodeProperty;
		GNode* condNode = globalASTTreePointer->findNodeByIndex(condNodeIndex);
		if(condNode->getTreeCode()=="le_expr" || condNode->getTreeCode()=="ge_expr")
		{
			result = NodeProcessor::getOperand(condNode,0)->getIndex();
		}
	}
	return result;
}
string
NodeProcessor::getFunctionCallName(GNode* node) throw (Error)
{
	if(node->getTreeCode()=="call_expr")
	{
		GNode* fnAddrNode = globalASTTreePointer->findNodeByIndex(
							node->getProperty("fn")->mNodeProperty
							);
		if(fnAddrNode->getTreeCode()=="addr_expr")
		{
			GNode *fnDeclNode = NodeProcessor::getOperand(fnAddrNode,0);
			if(fnDeclNode->getTreeCode()=="function_decl")
			{
				return FunctionCallNodeProcessor::getFunctionName(fnDeclNode);
			}
		}
	}
	throw Error();
}
//TODO need refinement & improvement
bool
NodeProcessor::isTypePrimitive(GNode* node) throw (Error)
{
	if(node->getTreeCode()=="integer_type" ||
	   node->getTreeCode()=="real_type"    ||
	   node->getTreeCode()=="enumeral_type"||
	   node->getTreeCode()=="pointer_type"
	)
	{
		return true;
	}
	return false;
}
bool
NodeProcessor::isVarRecordType(GNode* node) throw (Error)
{
	if(node->getTreeCode()!="var_decl" && node->getTreeCode()!="field_decl")
	{
		throw Error();
	}
	if(NodeProcessor::getVarType(node)=="record_type")
	{
		return true;
	}
	return false;
}
string
NodeProcessor::getVarTypeName(GNode* node) throw (Error)
{
	if(NodeProcessor::isVarRecordType(node))
	{
		GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
		GNode* typeDeclNode = globalASTTreePointer->findNodeByIndex(typeNode->getProperty("name")->mNodeProperty);
		GNode* idNode = globalASTTreePointer->findNodeByIndex(typeDeclNode->getProperty("name")->mNodeProperty);
		return idNode->getProperty("strg")->mStringProperty;
	}
	return "";
}
bool
NodeProcessor::isPointer(GNode* node) throw (Error)
{
	if(node->getTreeCode()=="var_decl")
	{
		GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
		if(typeNode->getTreeCode()=="pointer_type")
		{
			return true;
		}
		else
		{
			return false;
		}
		throw Error();
	}
}
bool
NodeProcessor::isVarPOD(GNode* node) throw (Error)
{
	if(node->getTreeCode()!="var_decl" && node->getTreeCode()!="field_decl")
	{
		throw Error();
	}
	GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
	return NodeProcessor::isTypePOD(typeNode);
}
bool
NodeProcessor::isTypePOD(GNode* node) throw (Error)
{
	if(NodeProcessor::isTypePrimitive(node))
	{
		return true;
	}
	GNode* typeDeclNode = globalASTTreePointer->findNodeByIndex(node->getProperty("name")->mNodeProperty);
	GNode* idNode = globalASTTreePointer->findNodeByIndex(typeDeclNode->getProperty("name")->mNodeProperty);
	return StructProcessor::getInstance().isStructPOD(idNode->getProperty("strg")->mStringProperty);
}

bool NodeProcessor::isRealConstant( GNode *node ) throw ( Error ){
	if ( "real_cst" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isGlobalScope( GNode *node ) throw ( Error ){
	if ( "translation_unit_decl" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isAndOrCondNode( GNode *node ) throw ( Error ){	
	if ( "truth_orif_expr" == node->getTreeCode() )
		return true;
	if ( "truth_andif_expr" == node->getTreeCode() )
		return true;
	return false;
}
bool NodeProcessor::isCompCondNode( GNode *node ) throw ( Error ){
        if ( "lt_expr" == node->getTreeCode() )
                return true;
	if ( "le_expr" == node->getTreeCode() )
                return true;
        if ( "gt_expr" == node->getTreeCode() )
                return true;
	if ( "ge_expr" == node->getTreeCode() )
                return true;
        if ( "eq_expr" == node->getTreeCode() )
                return true;
        if ( "ne_expr" == node->getTreeCode() )
                return true;
	return false;
}

bool NodeProcessor::isVariable( GNode *node ) throw ( Error ){
	if ( "var_decl" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isCleanupExpr( GNode *node ) throw ( Error ){
	if ( "cleanup_point_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isConvertExpr( GNode *node ) throw ( Error ){
	if ( "convert_expr" == node->getTreeCode() )
		return true;
	return false; 
}

bool NodeProcessor::isCompoundExpr( GNode *node ) throw ( Error ){
	if ( "compound_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isModifyStmt( GNode *node ) throw ( Error ){
	if ( "modify_expr" == node->getTreeCode() )
		return true;
	if ( "preincrement_expr" == node->getTreeCode() )
		return true;
	if ( "postincrement_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isModifyExpr( GNode *node ) throw ( Error ){
	if ( "modify_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isExprStmt( GNode *node ) throw ( Error ){
	if ( "expr_stmt" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isForStmt( GNode *node ) throw ( Error ){
        if ( "for_stmt" == node->getTreeCode() )
                return true;
        return false;
}

bool NodeProcessor::isTheSameType( GNode *node1, GNode *node2 ) throw (Error ){
	if ( node1->getTreeCode() == node2->getTreeCode() )
		return true;
	return false;
}
