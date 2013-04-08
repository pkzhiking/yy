/*
 * NodeProcessor.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include "include/NodeProcessor.h"
#include "include/ASTTree.h"
#include "include/FunctionCallNodeProcessor.h"
#include "include/StructProcessor.h"
#include "include/Expr.h"
#include "../util/include/Util.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <queue>
#include <map>

using std::endl;
using std::cerr;
using std::string;
using std::stringstream;
using std::boolalpha;
using std::queue;
using std::map;

struct sTreeCode
{
    string m_strType;
    int m_iNargs;
    sTreeCode(string strType, int iNargs) {m_strType = strType; m_iNargs = iNargs;}
};

#define DEFTREECODE(SYM, STRING, TYPE, NARGS) sTreeCode(STRING, NARGS),
sTreeCode g_TreeCode[] =
{
#include "tree.def"
sTreeCode("useless_tree_code", -1)
};
#undef DEFTREECODE

extern GTree *globalASTTreePointer;
string
NodeProcessor::getVarName(GNode* node) throw (Error)
{
	if(node->getTreeCode().substr(node->getTreeCode().length()-4) == "decl")
	{
		const GProperty *name_property = node->getProperty("name");
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
	//todo: 段错误
	if (node->getProperty(string("op ")+Util::intToString(opIndex)) != NULL) {
		return globalASTTreePointer->findNodeByIndex(node->getProperty(string("op ")+Util::intToString(opIndex))->mNodeProperty);
	}else{
		return NULL;
	}

}

string NodeProcessor::getFieldStr( GNode *node, string fieldName ) throw ( Error ){
    
	return node->getProperty( fieldName )->toString();
}

GNode *NodeProcessor::getFieldNode( GNode *node, string fieldName ) throw ( Error ){
	if (node->getProperty( fieldName ) != NULL){
		return globalASTTreePointer->findNodeByIndex( node->getProperty( fieldName )->mNodeProperty );
	}else{
		return NULL;
	}
}

bool
NodeProcessor::ifIntConstant(GNode* node) throw (Error)
{
	if (node == NULL)
		return false;
	if(node->getTreeCode()=="integer_cst")
	{
		return true;
	}
	else{
		return false;
	}
}
string
NodeProcessor::getCalledFunctionName(GNode* node) throw (Error)
{
	GNode* functionDecl;
		GNode* functionName;
		if (node->getChilds().size() > 1)
		{
		  functionDecl = globalASTTreePointer->findNodeByIndex(node->getChilds()[1]);
		  if (functionDecl->getChilds().size() > 1)
		  {
			  functionDecl = globalASTTreePointer->findNodeByIndex(functionDecl->getChilds()[1]);
		  }
		  else
		  {
			  return "";
		  }
		}
		else
		{
			return "";
		}


		string sName;
		if (functionDecl->getProperty("name") != NULL && functionDecl->getProperty("srcp")){

			sName = functionDecl->getProperty("srcp")->mStringProperty;
			if (sName.find(".") == string::npos)
				return "";
			sName = sName.substr(0, sName.find("."));


			functionName = globalASTTreePointer->findNodeByIndex(functionDecl->getProperty("name")->mNodeProperty);

			if (functionName->getProperty("strg") != NULL)
			{
				sName += "$&$" + functionName->getProperty("strg")->mStringProperty;
				return sName;
			}
		}


		return "";
}
bool
NodeProcessor::isMemApplyFunction(GNode* node) throw (Error)
{
	GNode* functionDecl;
	GNode* functionName;
	if (node->getChilds().size() > 1)
	{
	  functionDecl = globalASTTreePointer->findNodeByIndex(node->getChilds()[1]);
	  if (functionDecl->getChilds().size() > 1)
	  {
		  functionDecl = globalASTTreePointer->findNodeByIndex(functionDecl->getChilds()[1]);
	  }
	  else
	  {
		  return false;
	  }
	}
	else
	{
		return false;
	}


	if (functionDecl->getProperty("name") != NULL){
		functionName = globalASTTreePointer->findNodeByIndex(functionDecl->getProperty("name")->mNodeProperty);
		string sName = "";
		if (functionName->getProperty("strg") != NULL)
		{
			sName = functionName->getProperty("strg")->mStringProperty;
		}
		if (sName == "malloc" || sName == "calloc" || sName == "realloc")
		{
			return true;
		}
	}


	return false;
}
bool
NodeProcessor::isStrCopyFunction(GNode* node) throw (Error)
{
		GNode* functionDecl;
		GNode* functionName;
		if (node->getChilds().size() > 1)
		{
		  functionDecl = globalASTTreePointer->findNodeByIndex(node->getChilds()[1]);
		  if (functionDecl->getChilds().size() > 1)
		  {
			  functionDecl = globalASTTreePointer->findNodeByIndex(functionDecl->getChilds()[1]);
		  }
		  else
		  {
			  return false;
		  }
		}
		else
		{
			return false;
		}


		if (functionDecl->getProperty("name") != NULL){
			functionName = globalASTTreePointer->findNodeByIndex(functionDecl->getProperty("name")->mNodeProperty);
			string sName = "";
			if (functionName->getProperty("strg") != NULL)
			{
				sName = functionName->getProperty("strg")->mStringProperty;
			}
			if (sName == "strcpy" || sName == "strncpy")
			{
				return true;
			}
		}


		return false;
}
int
NodeProcessor::isInLoop(const vector<int>& context) throw (Error)
{
	int loopCounter = 0;
	GNode* node;
	for(unsigned int index = 0; index < context.size(); index++)
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
		//todo:段错误
			if (tmpNode == NULL){
				return NULL;
			}
		while(tmpNode->getTreeCode()!="var_decl")
		{
			tmpNode = NodeProcessor::getOperand(tmpNode,0);
			//todo:段错误
			if (tmpNode == NULL){
				break;
			}
		}
		return tmpNode;
	}
	//todo: 段错误
	//throw Error();
}
int
NodeProcessor::inferForLoopLooperVarNodeIndex(GNode* node) throw (Error)
{
	int result = -1;
	if(node->getTreeCode() == "for_stmt")
	{
		if ( node->getProperty("cond") == NULL)
			return 0;
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
	if (node != NULL){
		if ( "truth_orif_expr" == node->getTreeCode() )
			return true;
		if ( "truth_andif_expr" == node->getTreeCode() )
			return true;
	}
	return false;
}
bool NodeProcessor::isCompCondNode( GNode *node ) throw ( Error ){
	if (node != NULL){
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
	}
	return false;
}

bool NodeProcessor::isVariable( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	string strTreeType = node->getTreeCode();
	if ( "var_decl" == strTreeType || "field_decl" == strTreeType || "parm_decl" == strTreeType)
		return true;
	return false;
}

bool NodeProcessor::isCleanupExpr( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "cleanup_point_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isConvertExpr( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "convert_expr" == node->getTreeCode() )
		return true;
	return false; 
}

bool NodeProcessor::isCompoundExpr( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "compound_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isModifyStmt( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "modify_expr" == node->getTreeCode() )
		return true;
	if ( "preincrement_expr" == node->getTreeCode() )
		return true;
	if ( "postincrement_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isModifyExpr( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "modify_expr" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isExprStmt( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
	if ( "expr_stmt" == node->getTreeCode() )
		return true;
	return false;
}

bool NodeProcessor::isForStmt( GNode *node ) throw ( Error ){
	if (node == NULL){
		return false;
	}
        if ( "for_stmt" == node->getTreeCode() )
                return true;
        return false;
}

bool NodeProcessor::isTheSameType( GNode *node1, GNode *node2 ) throw (Error ){
	if (node1 != NULL && node2 != NULL)
	{
	if ( node1->getTreeCode() == node2->getTreeCode() )
		return true;
	}
	return false;
}
int NodeProcessor::getConditionVarID(GNode* node)throw (Error ){


	if (node->getProperty("op 0") == NULL)
		return -1;
		GNode* opNode = globalASTTreePointer->findNodeByIndex(node->getProperty("op 0")->mNodeProperty);

		if (opNode != NULL){
				if (opNode->getTreeCode() != "eq_expr"){
					return -1;
				}
				if (opNode->getProperty("op 0") == NULL)
					return -1;
				return opNode->getProperty("op 0")->mNodeProperty;
		}


		return -1;

}
int NodeProcessor::subtreeContainFunction(GNode* node, string nodeCode, string functionName) throw (Error){

		if (node->getProperty("cond") != NULL){
			node =globalASTTreePointer->findNodeByIndex(node->getProperty("cond")->mNodeProperty);
		}else{
			return -1;
		}

	  queue < int > subTreeQ;
	  vector < int > childs = node->getChilds();
	  map < int, bool> hashMap;
	  GNode* tNode;
	  for(int i = 0; i < childs.size(); i++){
		   hashMap[childs[i]] = true;
		   subTreeQ.push(childs[i]);
	  	  }
    while(!subTreeQ.empty()){
    	int id = subTreeQ.front();
    	subTreeQ.pop();
    	tNode = globalASTTreePointer->findNodeByIndex(id);
    	if (tNode->getTreeCode() == nodeCode){
    			 if (NodeProcessor::isNamedFunction(tNode, functionName)){
    				 return globalASTTreePointer->findNodeByIndex(\
    						 globalASTTreePointer->findNodeByIndex(tNode->getProperty("0")->mNodeProperty)->getProperty("op 0")->mNodeProperty)\
    								 ->getProperty("op 0")->mNodeProperty;
    				 //return true;
    				}
    		}
    	childs = tNode->getChilds();
    	 for(int i = 0; i < childs.size(); i++){
    		 if (hashMap[childs[i]]){
    			 continue;
    		 	 	 }
			   hashMap[childs[i]] = true;
			   subTreeQ.push(childs[i]);
    		  	  }
          }
    return -1;
}
bool
NodeProcessor::isNamedFunction(GNode* node, string calledName) throw (Error)
{
		GNode* functionDecl;
		GNode* functionName;
		if (node->getChilds().size() > 1)
		{
		  functionDecl = globalASTTreePointer->findNodeByIndex(node->getChilds()[1]);
		  if (functionDecl->getChilds().size() > 1)
		  {
			  functionDecl = globalASTTreePointer->findNodeByIndex(functionDecl->getChilds()[1]);
		  }
		  else
		  {
			  return false;
		  }
		}
		else
		{
			return false;
		}


		if (functionDecl->getProperty("name") != NULL){
			functionName = globalASTTreePointer->findNodeByIndex(functionDecl->getProperty("name")->mNodeProperty);
			string sName = "";
			if (functionName->getProperty("strg") != NULL)
			{
				sName = functionName->getProperty("strg")->mStringProperty;
			}
			if (sName == calledName)
			{
				return true;
			}
		}


		return false;
}
bool NodeProcessor::isVarChanged(GNode* node, int varID)throw (Error){

			if(node->getProperty("body") == NULL){
				return false;
			}
			queue < int > subTreeQ;
		  vector < int > childs = node->getChilds();
		  map < int, bool> hashMap;
		  GNode* tNode;
		  for(int i = 0; i < childs.size(); i++){
			   hashMap[childs[i]] = true;
			   subTreeQ.push(childs[i]);
		  	  }
	    while(!subTreeQ.empty()){
						int id = subTreeQ.front();
						subTreeQ.pop();
						tNode = globalASTTreePointer->findNodeByIndex(id);
						if (tNode->getTreeCode() == "modify_expr" || tNode->getTreeCode() == "postincrement_expr" || tNode->getTreeCode() == "postdecrement_expr"){
							GNode* varNode = globalASTTreePointer->findNodeByIndex(tNode->getProperty("op 0")->mNodeProperty);
							if (varNode->getTreeCode() == "array_ref"){
										if (varNode->getProperty("op 0")->mNodeProperty == varID)
											return true;
								}else{
									if (varNode->getTreeCode() == "var_decl"){
										if (varNode->getIndex() == varID)
											return true;
									}
								}
						}
	    	childs = tNode->getChilds();
	    	 for(int i = 0; i < childs.size(); i++){
	    		 if (hashMap[childs[i]]){
	    			 continue;
	    		 	 	 }
				   hashMap[childs[i]] = true;
				   subTreeQ.push(childs[i]);
	    		  	  }
	          }
	return false;
}
bool NodeProcessor::isBindExpr(GNode *node) throw (Error) {
	if (node == NULL){
		return false;
	}
	if ("bind_expr" == node->getTreeCode())
		return true;
	else
		return false;
}

bool NodeProcessor::isCondExpr(GNode *node) throw (Error) {
	if (node == NULL){
		return false;
	}
	if ("cond_expr" == node->getTreeCode())
		return true;
	else
		return false;
}

bool NodeProcessor::isStatementList(GNode *node) throw (Error) {
	if (node == NULL){
		return false;
	}
	if ("statement_list" == node->getTreeCode())
		return true;
	else
		return false;
}

bool NodeProcessor::isLoopStmt(GNode *node) throw (Error) {
	if (node == NULL){
		return false;
	}
	if ("for_stmt" == node->getTreeCode() || "while_stmt" == node->getTreeCode()
			|| "do_stmt" == node->getTreeCode())
		return true;
	else
		return false;
}

bool NodeProcessor::isFunctionDecl(GNode *node) throw (Error) {
	if (node == NULL){
		return false;
	}
	if ("function_decl" == node->getTreeCode())
		return true;
	else
		return false;
}

bool NodeProcessor::isParamDecl(GNode *node) throw (Error) {
    if(node == NULL){
        return false;
    }
    if("parm_decl" == node->getTreeCode())
        return true;
    else 
        return false;
}

GNode *NodeProcessor::isForContainIf(GNode* node)throw (Error ){


	if (node->getProperty("body") == NULL)
		return NULL;
		GNode* opNode = globalASTTreePointer->findNodeByIndex(node->getProperty("body")->mNodeProperty);

		if (opNode != NULL){
				if (opNode->getTreeCode() != "cond_expr"){
					return NULL;
				}
				if (opNode->getProperty("op 0") != NULL)
				return globalASTTreePointer->findNodeByIndex(opNode->getProperty("op 0")->mNodeProperty);
		}


		return NULL;

}
int NodeProcessor::getConditonVar(GNode* node, string op)throw (Error ){

	if (node != NULL){
					if (node->getTreeCode().find("truth_") != string::npos){
						return -1;
					}
					if (node->getProperty(op) == NULL)
						return -1;
					GNode* varNode = globalASTTreePointer->findNodeByIndex(node->getProperty(op)->mNodeProperty);
					if (varNode->getTreeCode().find("var_") == string::npos){
						if (varNode->getTreeCode().find("array_ref") == string::npos){
							  return -1;
						}else{
							if (varNode->getProperty("op 0") != NULL)
							  return varNode->getProperty("op 0")->mNodeProperty;
						}
					}
					return node->getProperty(op)->mNodeProperty;
			}
			return -1;

}


bool NodeProcessor::isCurrentFunctionDecl(GNode *node) throw(Error){
			if (node->getProperty("name") == NULL)
				return false;
			 GNode* nameNode = globalASTTreePointer->findNodeByIndex(node->getProperty("name")->mNodeProperty);
			 if (nameNode->getProperty("strg") == NULL){
				 return false;
			 }


			 if (nameNode->getProperty("strg")->mStringProperty == globalASTTreePointer->_getFunctionName()){
				 return true;
			 }else{
				 return false;
			 }


}
GNode *NodeProcessor::getFuncTypeNode(GNode *node) throw(Error){
			if (node->getProperty("type") == NULL){
					return NULL;
				}
			return globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
}
bool NodeProcessor::isFunctionReturnRecordType(GNode *node) throw(Error){
		if (node->getProperty("retn") == NULL){
			return false;
		}
		GNode* returnNode = globalASTTreePointer->findNodeByIndex(node->getProperty("retn")->mNodeProperty);
		if (returnNode->getTreeCode() == "record_type"){
				return true;
		}else{
			return false;
		}
}
bool NodeProcessor::isFunctionPrmsRecordType(GNode *node) throw(Error){
		if (node->getProperty("prms") == NULL){
			return false;
		}
		GNode* prmsNode = globalASTTreePointer->findNodeByIndex(node->getProperty("prms")->mNodeProperty);
		while(true){
			if (prmsNode->getProperty("valu") == NULL){
				return false;
			}
			string prmsType = globalASTTreePointer->findNodeByIndex(prmsNode->getProperty("valu")->mNodeProperty)->getTreeCode();
			if (prmsType == "record_type"){
				return true;
			}
			if (prmsNode->getProperty("chan") == NULL){
				return false;
			}
			prmsNode = globalASTTreePointer->findNodeByIndex(prmsNode->getProperty("chan")->mNodeProperty);
		}


}
string NodeProcessor::getCondTesting(GNode *node) throw (Error)
{
    if(NULL == node)
	return "";
    if("cond_expr" != node->getTreeCode())
	return "";
    string testing = "";
    GNode* op0Node;
    if(NULL != node)
	op0Node = globalASTTreePointer->findNodeByIndex(node->getProperty("op 0")->mNodeProperty);
    GNode* op1Node;

    if(NULL == op0Node)
    {
	return "";
    }
    if("truth_andif_expr" != op0Node->getTreeCode() && "truth_orif_expr" != op0Node->getTreeCode())
	return getCompareOperation(op0Node);

    while("truth_andif_expr" == op0Node->getTreeCode() || "truth_orif_expr" == op0Node->getTreeCode())
    {
	if(NULL != op0Node)
	{
	    op0Node = globalASTTreePointer->findNodeByIndex(op0Node->getProperty("op 0")->mNodeProperty);
	    op1Node = globalASTTreePointer->findNodeByIndex(op0Node->getProperty("op 1")->mNodeProperty);
	    testing = op0Node->getTreeCode() + getCompareOperation(op1Node);
	}
    }
    testing = getCompareOperation(op0Node) + testing;
    return testing;
}

bool NodeProcessor::isCompareOperation(GNode *node) throw (Error)
{
    if("gt_expr" != node->getTreeCode() &&
	"ge_expr" != node->getTreeCode() &&
	"lt_expr" != node->getTreeCode() &&
	"le_expr" != node->getTreeCode() &&
	"eq_expr" != node->getTreeCode() &&
	"ne_expr" != node->getTreeCode())
	return false;
    return true;
}


string NodeProcessor::getCompareOperation(GNode *node) throw (Error)
{
    if("gt_expr" != node->getTreeCode() &&
	"ge_expr" != node->getTreeCode() &&
	"lt_expr" != node->getTreeCode() &&
	"le_expr" != node->getTreeCode() &&
	"eq_expr" != node->getTreeCode() &&
	"ne_expr" != node->getTreeCode())
	return "";
    GNode* op0Node = globalASTTreePointer->findNodeByIndex(node->getProperty("op 0")->mNodeProperty);
    GNode* op1Node = globalASTTreePointer->findNodeByIndex(node->getProperty("op 1")->mNodeProperty);
    return getSimpleOperation(op0Node) + node->getTreeCode() + getSimpleOperation(op1Node);
}

string NodeProcessor::getSimpleOperation(GNode *node) throw (Error)
{
    if("plus_expr" != node->getTreeCode() &&
	"minus_expr" != node->getTreeCode() &&
	"mult_expr" != node->getTreeCode() &&
	"trunc_div_expr" != node->getTreeCode() &&
	"var_expr" != node->getTreeCode() &&
	"integer_cst" != node->getTreeCode() &&
	"real_cst" != node->getTreeCode())
	return "";
    if("var_expr" == node->getTreeCode())
	return getVarName(node);
    if("integer_cst" == node->getTreeCode() || "real_cst" == node->getTreeCode())
	return getConstant(node);

    GNode* op0Node = globalASTTreePointer->findNodeByIndex(node->getProperty("op 0")->mNodeProperty);
    GNode* op1Node = globalASTTreePointer->findNodeByIndex(node->getProperty("op 1")->mNodeProperty);
    if(NULL == op0Node || NULL == op1Node)
	return "";
    
    return getSimpleOperation(op0Node) + node->getTreeCode() + getSimpleOperation(op1Node);
}

string NodeProcessor::getConstant(GNode *node) throw (Error)
{
    if("integer_cst" != node->getTreeCode() &&
	"real_cst" != node->getTreeCode())
	return "";
    return node->getProperty("low")->mStringProperty;
}

bool NodeProcessor::isLogicAnd(GNode *node) throw (Error)
{
    if("truth_andif_expr" == node->getTreeCode())
	return true;
    return false;
}

bool NodeProcessor::isLogicAnti(string left, string right) throw (Error)
{
    if("eq_expr" == left && "ne_expr" == right || "ne_expr" == left && "eq_expr" == right)
	return true;
    if("lt_expr" == left && "ge_expr" == right || "ge_expr" == left && "lt_expr" == right)
	return true;
    if("le_expr" == left && "gt_expr" == right || "gt_expr" == left && "le_expr" == right)
	return true;
}

//get the name of a node, the very name may need several searches down the chains
string NodeProcessor::GetName(GNode *gnNode) throw(Error)
{
    GTree *gnTree = gnNode->_getTree();
    map<int, GNode*> map_Node = gnTree->_getNodeMap();
    map<int, GNode*>::iterator iteGNode = map_Node.begin();
#if 0
    for(int i = 1; map_Node.end() != iteGNode; iteGNode++, i++)
    {
	if(gnNode == iteGNode->second)
	    cout << "The Node NO:" << i << endl;
	GNode *gnTemp = iteGNode->second;
	gnTemp->Print();
    }
#endif
    if("" != getFieldStr(gnNode, "identifier_node"))
    {
	return getFieldStr(gnNode, "strg");
    }
    string strName = getFieldStr(gnNode, "name");
    string strNameChild = strName;
    GNode *gnNameFather = gnNode;
    while("" != strNameChild)
    {
	strName = strNameChild;
	strNameChild.erase(0, 1);
	unsigned int uiNameChild = Util::stringToInt(strNameChild);
	GNode *gnNameChild = map_Node[uiNameChild];
	gnNameFather = gnNameChild;
	strNameChild = getFieldStr(gnNameChild, "name");
    }
    return getFieldStr(gnNameFather, "strg");
}
 
//Construct a var from var_decl or parm_decl or field_decl node from dump file 
CVar* NodeProcessor::ConstructVar(GNode *gnNode) throw(Error)
{
    GTree *gTree = gnNode->_getTree();
    map<int, GNode*> mapNode = gTree->_getNodeMap();
    if("var_decl" != gnNode->getTreeCode() && "parm_decl" != gnNode->getTreeCode() && "field_decl" != gnNode->getTreeCode())
	return 0;
    CVar *pVar = new CVar;
    pVar->SetNo(gnNode->getIndex());
    pVar->SetTreeType(gnNode->getTreeCode());
    pVar->SetName(NodeProcessor::GetName(gnNode));
    string strNodeType = NodeProcessor::getFieldStr(gnNode, "type");
    strNodeType.erase(0, 1);
    unsigned int uiTypeNode = Util::stringToInt(strNodeType);
    
    pVar->SetType(mapNode[uiTypeNode]);
    pVar->SetSize(NodeProcessor::GetSize(gnNode));
    if(0 > GetAlgn(gnNode))
    {
	cout << "Error to get align of var" << endl;
	return NULL;
    }
    pVar->SetAlgn(GetAlgn(gnNode));
    return pVar;
}

CConstant* NodeProcessor::ConstructConst(GNode *gnNode) throw(Error)
{
    GTree *gTree = gnNode->_getTree();
    map<int, GNode*> mapNode = gTree->_getNodeMap();
    if("integer_cst" != gnNode->getTreeCode() && "real_cst" != gnNode->getTreeCode())
	return 0;
    CConstant *pConst = new CConstant;
    pConst->SetNo(gnNode->getIndex());
    pConst->SetTreeType(gnNode->getTreeCode());
    pConst->SetValue(GetValue(gnNode));
    return pConst;
}

int NodeProcessor::GetSize(GNode *gnNode) throw(Error)
{
    GTree *gnTree = gnNode->_getTree();
    map<int, GNode*> mapNode = gnTree->_getNodeMap();
    string strSizeNode = NodeProcessor::getFieldStr(gnNode, "size");
    if("" == strSizeNode)
	return -1;
    strSizeNode.erase(0, 1);
    unsigned int uiSizeNode = Util::stringToInt(strSizeNode);
    GNode *gnSizeNode = mapNode[uiSizeNode];
    string strSize = NodeProcessor::getFieldStr(gnSizeNode, "low");
    if("" == strSize)
	return -1;
    return Util::stringToInt(strSize);
}

int NodeProcessor::GetAlgn(GNode *gnNode) throw(Error)
{
    string strAlgn = getFieldStr(gnNode, "algn");
    if("" == strAlgn)
	return -1;
    return Util::stringToInt(strAlgn);
}

UValue NodeProcessor::GetValue(GNode *gnNode) throw(Error)
{
    UValue uValue;
    string strType = gnNode->getTreeCode();
    if("integer_cst" == strType)
    {   
	uValue.m_iValue = Util::stringToInt(NodeProcessor::getFieldStr(gnNode, "low"));
    }   
    else
    {   
        if("real_cst" == strType)
        {   
            uValue.m_dValue = Util::stringToDouble(NodeProcessor::getFieldStr(gnNode, "low"));
        }   
    }   
    return uValue;
}

void NodeProcessor::PrintTreeCode() throw(Error)
{
    for(int i = 0; "useless_tree_code" != g_TreeCode[i].m_strType; i++)
    {
	cout << "TreeCode:" << g_TreeCode[i].m_strType << "\tnArgs:" << g_TreeCode[i].m_iNargs << endl;
    }
}

int NodeProcessor::GetNArgs(GNode *gnNode) throw(Error)
{
    string strType = gnNode->getTreeCode();
    if("" == strType)
	return -1;
    for(int i = 0; "useless_tree_code" != g_TreeCode[i].m_strType; i++)
    {
	if(strType == g_TreeCode[i].m_strType)
	    return g_TreeCode[i].m_iNargs;
    }
    return -1;
}

CExpr* NodeProcessor::GetExpr(GNode *gnNode) throw(Error)
{
    int iArgs = GetNArgs(gnNode);
    if(iArgs < 0)
    {
	return 0;
    }
    return 0;    
}

//simple expr include variable, parameter, constant and other objects whose TreeType end with decl
bool NodeProcessor::IsSimpleExpr(GNode *gnNode) throw(Error)
{
    string strTreeType = gnNode->getTreeCode();
    if(strTreeType.size() < 4)
	return false;
    cout << strTreeType << endl;
    string strLast4Char = strTreeType.substr(strTreeType.size() - 4, 4);
    if(0 == strLast4Char.compare("decl") || 0 == strLast4Char.compare("_cst"))
	return true;
    return false;
}

bool NodeProcessor::IsConst(GNode *gnNode) throw(Error)
{
    string strTreeType = gnNode->getTreeCode();
    if(0 == strTreeType.substr(strTreeType.size() - 4, 4).compare("_cst"))
	return true;
    return false;
}

void* NodeProcessor::GetSimpleExpr(GNode *gnNode) throw(Error)
{
    if(!IsSimpleExpr(gnNode))
	return 0;
    if(isVariable(gnNode))
	return (void*) ConstructVar(gnNode);
    else
	if(IsConst(gnNode))
	    return (void*) ConstructConst(gnNode);
    return 0;
}

bool NodeProcessor::IsExpr(GNode *pNode) throw(Error)
{
    string strTreeCode = pNode->getTreeCode();
    string strLast4Char = strTreeCode.substr(strTreeCode.size() - 4, 4);
    if(0 == strLast4Char.compare("expr") || 0 == strLast4Char.compare("_ref"))
	return true;
    return false;
}

CUnit* NodeProcessor::ConstructExpr(GNode* pNode) throw(Error)
{
    if(!pNode || (!IsExpr(pNode) && !IsSimpleExpr(pNode)))
	return 0;
    CExpr *pExpr = new CExpr;
    if(IsSimpleExpr(pNode))
    {
	if(isVariable(pNode))
	    return (CUnit*) ConstructVar(pNode);
	else
	    return (CUnit*) ConstructConst(pNode);
    }
    else
    {
	string strTreeType = pNode->getTreeCode();
	unsigned int uiNArgs = GetNArgs(pNode);
	pExpr->SetOp(SOp(strTreeType, uiNArgs));
	for(int i = 0; i < uiNArgs; i++)
	{
	    GNode *pGnOperand = getOperand(pNode, i);
	    CUnit *pOperand = ConstructExpr(pGnOperand);
#if 0
	    cout << "EXPR:" << endl;
	    switch(((CUnit*) pOperand)->m_unit)
	    {
		case CUnit::VAR:
		    ((CVar*) pOperand)->Print();
		    break;
		case CUnit::CONST:
		    ((CConstant*) pOperand)->Print();
		    break;
		default:
		    break;
	    }
#endif
	    pExpr->AddOperand(pOperand);
	}
	return (CUnit*) pExpr;
    }
}

int NodeProcessor::GetNextBindIndex(GNode *pGnNode) throw(Error)
{
    GTree *pTree = pGnNode->_getTree();
    int iIndex = pGnNode->getIndex();
    map<int, GNode*> map_node = pTree->_getNodeMap();
    map<int, GNode*>::iterator ite_mapNode = map_node.begin();
    for(int i = 0; i < iIndex; i++)
	ite_mapNode++;
    while(map_node.end() != ite_mapNode && !isBindExpr(ite_mapNode->second))
	ite_mapNode++;
    if(map_node.end() == ite_mapNode)
	return -1;
    else
	return ite_mapNode->first;
}

int NodeProcessor::GetNextBindFirstVarIndex(GNode *pGnNode) throw(Error)
{
    GTree *pTree = pGnNode->_getTree();
    map<int, GNode*> map_node = pTree->_getNodeMap();
    int iNextBindIndex = GetNextBindIndex(pGnNode);
    while(iNextBindIndex > 0)
    {
	GNode *pNextBindNode = map_node[iNextBindIndex];
	string strNode = getFieldStr(pNextBindNode, "vars");
	if("" != strNode)
	{
	    cout << "vars:vars:" << strNode << endl;
	    strNode.erase(0, 1);
	    return Util::stringToInt(strNode);
	}
	iNextBindIndex = GetNextBindIndex(pNextBindNode);
    }
    return -1;
}

bool NodeProcessor::IsTruthExpr(GNode *pNode) throw(Error)
{
    if("truth_andif_expr" == pNode->getTreeCode() || "truth_orif_expr" == pNode->getTreeCode())
	return true;
    return false;
}
