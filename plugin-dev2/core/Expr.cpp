#include "include/Expr.h"
#include <iostream>
#include "include/NodeProcessor.h"
#include "../util/include/Util.h"

using std::cout;
using std::endl;

bool CExpr::operator==(CUnit &right)
{
    CExpr *pRight = (CExpr*) (&right);
    if(m_op != pRight->m_op)
	return false;
    for(int i = 0; i < m_op.m_uiNArgs; i++)
    {
	if(!(m_operands[i]->Equal((pRight->m_operands[i]))))
	    return false;
	if(!(*m_operands[i] == *(pRight->m_operands[i])))
	    return false;
    }
    return true;
}

void CExpr::Print()
{
    for(int i = 0; i < m_op.m_uiNArgs; i++)
    {
	(m_operands[i])->Print();
    }
    cout << m_op.m_strOp << m_op.m_uiNArgs << endl;
}

vector<CVar*> CExpr::CollectVars()
{
    vector<CVar*> vec_var;
    for(int i = 0; i < m_op.m_uiNArgs; i++)
    {
	if(VAR == m_operands[i]->GetUnit())
	    vec_var.push_back((CVar*) m_operands[i]);
	if(EXPR == m_operands[i]->GetUnit())
	{
	    vector<CVar*> vec_operandVar = ((CExpr*) m_operands[i])->CollectVars();
	    vec_var.insert(vec_var.end(), vec_operandVar.begin(), vec_operandVar.end());
	}
    }
    return vec_var;
}

void CConstant::Print() 
{
    cout << "@" << m_uiNo << "\tTreeType:" << m_strTreeType << "\tvalue:";
    switch(m_type)
    {
        case INTEGER:
	    cout << m_uValue.m_iValue;
	    break;
	case REAL:
	    cout << m_uValue.m_dValue;
	    break;
	default:
	    break;
    }
    cout << endl;
}

void CVar::Print()
{
    cout << "@" << m_uiNo << "\tname:" << m_strName << "\tTreeType:" << m_strTreeType << "\tsize:" << m_uiSize << "\tAlgn:" << m_uiAlgn << endl;
}

CExpr* CCondStmt::ConstructTesting(GNode *pGnNode)
{
    return (CExpr*) NodeProcessor::ConstructExpr(pGnNode);
}

vector<CVar*> CCondStmt::AnalyzeChangedVar()
{
    vector<CVar*> vec_var;
    vector<CVar*> vec_right;
    vec_var = m_then.AnalyzeChangedVar();
    cout << "PRINTING VECTOR THEN PART." << endl;
    PrintVectorVar(vec_var);
    vec_right = m_else.AnalyzeChangedVar();
    cout << "PRINTING VECTOR ELSE PART." << endl;
    PrintVectorVar(vec_var);
    vec_var.insert(vec_var.end(), vec_right.begin(), vec_right.end());
    return vec_var;
}

bool CCondStmt::IsTestingVarChangedInThen()
{
    vector<CVar*> vec_thenVar = AnalyzeChangedVar();
    cout << "print testing vars" << endl;
    for(int i = 0; i < m_testingVars.size(); i++)
    {
	m_testingVars[i]->Print();
    }
    cout << "print contents vars" << endl;
    for(int i = 0; i < vec_thenVar.size(); i++)
    {
	vec_thenVar[i]->Print();
    }
    cout << "end" << endl;
    for(int i = 0; i < m_testingVars.size(); i++)
    {
	for(int j = 0; j < vec_thenVar.size(); j++)
	{
	    CVar testing = *m_testingVars[i];
	    CVar then = *vec_thenVar[j];
	    if(testing == then)
		return true;
	}
    }
    return false;
}

void CStmtList::ConstructStmtList(GNode *pGnNode)
{
    if(!NodeProcessor::isStatementList(pGnNode))
	return;
    int iOperand = 0;
    CStmt *pStmt;
    //cout << "IOPERAND2" << NodeProcessor::getFieldStr(pGnNode, "0") << endl;
    while("" != NodeProcessor::getFieldStr(pGnNode, Util::intToString(iOperand)))
    {
	string strOperand = NodeProcessor::getFieldStr(pGnNode, Util::intToString(iOperand));
	strOperand.erase(0, 1);
	int iChild = Util::stringToInt(strOperand);
	GTree *pTree = pGnNode->_getTree();
	map<int, GNode*> map_node = pTree->_getNodeMap();
	iOperand++;
	if("decl_expr" == map_node[iChild]->getTreeCode())
	    continue;
	pStmt = pStmt->ConstructStmt0(map_node[iChild]);
	if(pStmt)
	    m_vecStmt.push_back(pStmt);
    }
    return;
}

CStmt* CCondStmt::ConstructStmt(GNode *pGnNode)
{
    if("cond_expr" != pGnNode->getTreeCode())
	return 0;
    m_testing = (CTesting*) ConstructTesting(NodeProcessor::getOperand(pGnNode, 0));
    GNode *pLeft = NodeProcessor::getOperand(pGnNode, 1);
    m_then.ConstructBind(pLeft);
    m_then.Print();
    m_then.AnalyzeChangedVar();
    GNode *pRight = NodeProcessor::getOperand(pGnNode, 2);
    m_else.ConstructBind(pRight);
    m_testingVars = m_testing->CollectVars();
}

void CBind::ConstructVars(GNode *pGnNode)
{
    if(!NodeProcessor::isBindExpr(pGnNode))
	return;
    string strFirstVar = NodeProcessor::getFieldStr(pGnNode, "vars");
    if("" == strFirstVar)
	return;
    strFirstVar.erase(0, 1);
    int iFirstVarIndex = Util::stringToInt(strFirstVar);
    int iNextBindFirstVar = NodeProcessor::GetNextBindFirstVarIndex(pGnNode);
    GTree *pTree = pGnNode->_getTree();
    map<int, GNode*> map_node = pTree->_getNodeMap();
    map<int, GNode*>::iterator ite_mapNode = map_node.begin();
    for(int i = 0; i < iFirstVarIndex - 1; i++)
	ite_mapNode++;
    for(int i = iFirstVarIndex; map_node.end() != ite_mapNode && ( iNextBindFirstVar < 0 || i < iNextBindFirstVar); i++, ite_mapNode++)
    {
	if("var_decl" == ite_mapNode->second->getTreeCode())
	{
	    CVar *pVar = NodeProcessor::ConstructVar(ite_mapNode->second);
	    m_vecVar.push_back(pVar);
	}
    }
}

CBind* CBind::ConstructBind(GNode *pGnNode)
{
    if(!NodeProcessor::isBindExpr(pGnNode))
	return 0;
    ConstructVars(pGnNode);
    string strBody = NodeProcessor::getFieldStr(pGnNode, "body");
    if("" == strBody)
	return 0;
    strBody.erase(0, 1);
    int iBody = Util::stringToInt(strBody);
    GTree *pTree = pGnNode->_getTree();
    map<int, GNode*> map_Node = pTree->_getNodeMap();
    if("statement_list" == map_Node[iBody]->getTreeCode())
	m_stmtList.ConstructStmtList(map_Node[iBody]);
    else
    {
	CStmt *pStmt;
	pStmt = pStmt->ConstructStmt0(map_Node[iBody]);
	if(pStmt)
	    m_stmtList.AddStmt(pStmt);
    }
    AnalyzeChangedVar();
    return this;
}

vector<CVar*> CBind::AnalyzeChangedVar()
{
    for(int i = 0; i < m_stmtList.Size(); i++)
    {
	CStmt *pStmt = m_stmtList.GetOperand(i);
	vector<CVar*> vec_var = pStmt->AnalyzeChangedVar();
	m_vecChanged.insert(m_vecChanged.end(), vec_var.begin(), vec_var.end());
	//for(int j = 0; j < vec_var.size(); j++)
	//{
	//    m_vecChanged.push_back(vec_var[j]);
	//}
    }
    cout << "PRINTING IN BIND VAR" << endl;
    PrintVectorVar(m_vecChanged);
    return m_vecChanged;
}

void CBind::Print()
{
    cout << "BIND DUMP BEGIN" << endl;
    for(int i = 0; i < m_vecVar.size(); i++)
    {
	m_vecVar[i]->Print();
    }
    m_stmtList.Print();
    cout << "var changed in bind" << endl;
    for(int i = 0; i < m_vecChanged.size(); i++)
    {
	m_vecChanged[i]->Print();
    }
    cout << "BIND DUMP END." << endl;
}

void CStmtList::Print()
{
    cout << "STATEMENT_LIST START" << endl;
    for(int i = 0; i < m_vecStmt.size(); i++)
	m_vecStmt[i]->Print();
    cout << "STATEMENT_LIST END" << endl;
}

CStmt* CStmt::ConstructStmt0(GNode *pNode)
{
    if("bind_expr" == pNode->getTreeCode())
	return 0;
    if("return_expr" == pNode->getTreeCode())
	return 0;
    if("cleanup_point_expr" == pNode->getTreeCode())
    {
	return CCleanupStmt::ConstructCleanupStmt(pNode);
    }
    if("cond_expr" == pNode->getTreeCode())
    {
	CCondStmt *pCondStmt = new CCondStmt;
	pCondStmt->ConstructStmt(pNode);
	return (CStmt*) pCondStmt;
    }
    if("modify_expr" == pNode->getTreeCode())
    {
	CModifyStmt *pModifyStmt = new CModifyStmt;
	pModifyStmt->ConstructStmt(pNode);
	return (CStmt*) pModifyStmt;
    }
    m_pStmt = (CExpr*) NodeProcessor::ConstructExpr(pNode);
    return this;
}

void CCondStmt::Print()
{
    cout << "testing" << endl;
    m_testing->Print();
    cout << "testing vars" << endl;
    for(int i = 0; i < m_testingVars.size(); i++)
    {
	m_testingVars[i]->Print();
    }
    cout << "then" << endl;
    m_then.Print();
    cout << "else" << endl;
    m_else.Print();
    if(IsTestingVarChangedInThen())
	cout << "testing var changed in then" << endl;
    else
	cout << "testing var is not changed in then." << endl;
}

CStmt* CModifyStmt::ConstructStmt(GNode *pNode)
{
    if("modify_expr" != pNode->getTreeCode())
	return 0;
    GTree *pTree = pNode->_getTree();
    map<int, GNode*> map_node = pTree->_getNodeMap();
    GNode *pLeft = NodeProcessor::getOperand(pNode, 0);
    GNode *pRight = NodeProcessor::getOperand(pNode, 1);
    if(0 == pLeft || 0 == pRight)
	return 0;
    m_left = NodeProcessor::ConstructVar(pLeft);
    m_left->SetChanged(1);
    m_right = (CExpr*) NodeProcessor::ConstructExpr(pRight);
    return this;
}

void CModifyStmt::Print()
{
    cout << "DUMP MOFIDY BEGIN" << endl;
    m_left->Print();
    m_right->Print();
    cout << "DUMP MODIFY END" << endl;
}

//The simple statement contained in statement-list in the source program is organized as
//the structure of cleanup_point_expr, a typical organization is such as:
//cleanup_point_expr --> expr_stmt --> convert_expr --> actual stmt (like mofify_expr)
CStmt* CCleanupStmt::ConstructCleanupStmt(GNode *pNode)
{
    if("cleanup_point_expr" != pNode->getTreeCode())
	return 0;
    GNode *pExprStmt = NodeProcessor::getOperand(pNode, 0);
    if(0 == pExprStmt)
	return 0;
    string strLine = NodeProcessor::getFieldStr(pExprStmt, "line");
    int iLine = Util::stringToInt(strLine);
    string strExpr = NodeProcessor::getFieldStr(pExprStmt, "expr");
    if("" == strExpr)
	return 0;
    strExpr.erase(0, 1);
    int iExpr = Util::stringToInt(strExpr);
    GTree *pTree = pNode->_getTree();
    map<int, GNode*> map_node = pTree->_getNodeMap();
    GNode *pStmt = map_node[iExpr];
    if("convert_expr" == pStmt->getTreeCode())
    {
	GNode *pConvert = NodeProcessor::getOperand(pStmt, 0);
	if(0 == pConvert)
	    return 0;
	if("modify_expr" == pConvert->getTreeCode())
	{
	    CModifyStmt *pModifyStmt = new CModifyStmt;
	    pModifyStmt->ConstructStmt(pConvert);
	    pModifyStmt->SetLine(iLine);
	    return ((CStmt*) pModifyStmt);
	}
	if("preincrement_expr" == pConvert->getTreeCode())
	{
	    CPreIncreStmt *pPreIncreStmt = new CPreIncreStmt;
	    pPreIncreStmt->ConstructStmt(pConvert);
	    pPreIncreStmt->SetLine(iLine);
	    return ((CStmt*) pPreIncreStmt);
	}
	if("postincrement_expr" == pConvert->getTreeCode())
	{
	    CPostIncreStmt *pPostIncreStmt = new CPostIncreStmt;
	    pPostIncreStmt->ConstructStmt(pConvert);
	    pPostIncreStmt->SetLine(iLine);
	    return ((CStmt*) pPostIncreStmt);
	}
	if("predecrement_expr" == pConvert->getTreeCode())
	{
	    CPreDecreStmt *pPreDecreStmt = new CPreDecreStmt;
	    pPreDecreStmt->ConstructStmt(pConvert);
	    pPreDecreStmt->SetLine(iLine);
	    return ((CStmt*) pPreDecreStmt);
	}
	if("postdecrement_expr" == pConvert->getTreeCode())
	{
	    CPostDecreStmt *pPostDecreStmt = new CPostDecreStmt;
	    pPostDecreStmt->ConstructStmt(pConvert);
	    pPostDecreStmt->SetLine(iLine);
	    return ((CStmt*) pPostDecreStmt);
	}
    }
    return 0;
}

void CPreIncreStmt::Print()
{
    cout << "PRE INCREMENT STMT." << endl;
    m_pVar->Print();
}

CStmt* CPreIncreStmt::ConstructStmt(GNode* pNode)
{
    if("preincrement_expr" != pNode->getTreeCode())
	return 0;
    GNode *pVar = NodeProcessor::getOperand(pNode, 0);
    if(0 == pVar)
	return 0;
    m_pVar = NodeProcessor::ConstructVar(pVar);
    m_pVar->SetChanged(1);
    return this;
}

void CPostIncreStmt::Print()
{
    cout << "POST INCREMENT STMT." << endl;
    m_pVar->Print();
}

CStmt* CPostIncreStmt::ConstructStmt(GNode* pNode)
{
    if("postincrement_expr" != pNode->getTreeCode())
	return 0;
    GNode *pVar = NodeProcessor::getOperand(pNode, 0);
    if(0 == pVar)
	return 0;
    m_pVar = NodeProcessor::ConstructVar(pVar);
    m_pVar->SetChanged(1);
    return this;
}

void CPreDecreStmt::Print()
{
    cout << "PRE DECREMENT STMT." << endl;
    m_pVar->Print();
}

CStmt* CPreDecreStmt::ConstructStmt(GNode* pNode)
{
    if("predecrement_expr" != pNode->getTreeCode())
	return 0;
    GNode *pVar = NodeProcessor::getOperand(pNode, 0);
    if(0 == pVar)
	return 0;
    m_pVar = NodeProcessor::ConstructVar(pVar);
    m_pVar->SetChanged(1);
    return this;
}

void CPostDecreStmt::Print()
{
    cout << "POST DECREMENT STMT." << endl;
    m_pVar->Print();
}

CStmt* CPostDecreStmt::ConstructStmt(GNode* pNode)
{
    if("postdecrement_expr" != pNode->getTreeCode())
	return 0;
    GNode *pVar = NodeProcessor::getOperand(pNode, 0);
    if(0 == pVar)
	return 0;
    m_pVar = NodeProcessor::ConstructVar(pVar);
    m_pVar->SetChanged(1);
    return this;
}

void PrintVectorVar(vector<CVar*> vec_var)
{
    for(int i = 0; i < vec_var.size(); i++)
    {
	vec_var[i]->Print();
    }
}
