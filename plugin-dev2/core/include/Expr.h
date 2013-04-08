#ifndef IFTOELSEANALYZER_H_H
#define IFTOELSEANALYZER_H_H

#include "BaseAnalyzer.h"
#include <vector>
#include <string>
#include "ASTTree.h"
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

class CUnit
{
public:
    enum eUNIT {VAR, CONST, EXPR, STMT, CONDSTMT, STMTLIST, BIND, MODIFYSTMT, PREINCRESTMT, POSTINCRESTMT, PREDECRESTMT, POSTDECRESTMT};
public:
    eUNIT m_unit;
    eUNIT GetUnit() {return m_unit;}
    void SetUnit(eUNIT unit) {m_unit = unit;}
    virtual void Print() = 0;
    bool Equal(CUnit *pRight)
    {
	if(m_unit == pRight->m_unit)
	    return *this == *pRight;
	else
	    return false;
    }
    virtual bool operator==(CUnit&) = 0;
};

class CVar: public CUnit
{
private:
    GNode *m_gnType;
    string m_strName;
    string m_strTreeType;
    unsigned int m_uiNo;
    unsigned int m_uiSize;
    unsigned int m_uiAlgn;
    bool m_bChanged;
public:
    CVar(bool bChanged = 0, GNode *gnType = 0, string strName = "", unsigned int uiNo = 0, unsigned int uiSize = 0, unsigned int uiAlgn = 0, string strTreeType = "var_decl"): m_gnType(gnType), m_strName(strName), m_strTreeType(strTreeType), m_uiNo(uiNo), m_uiSize(uiSize), m_uiAlgn(uiAlgn), m_bChanged(bChanged) {SetUnit(VAR);}
    string GetName() {return m_strName;}
    void SetName(string strName) {m_strName = strName;}
    string GetTreeType() {return m_strTreeType;}
    void SetTreeType(string strTreeType) {m_strTreeType = strTreeType;}
    unsigned int GetNo() {return m_uiNo;}
    void SetNo(unsigned int uiNo) {m_uiNo = uiNo;}
    void SetType(GNode* gnType) {m_gnType = gnType;}
    GNode* GetType() {return m_gnType;}
    unsigned int Size() {return m_uiSize;}
    void SetSize(unsigned int uiSize) {m_uiSize = uiSize;}
    unsigned int GetAlgn() {return m_uiAlgn;}
    void SetAlgn(unsigned int uiAlgn) {m_uiAlgn = uiAlgn;}
    void Print();
    bool operator==(CUnit &right) {return m_uiNo == ((CVar*) &right)->m_uiNo;}
    bool GetChanged() {return m_bChanged;}
    void SetChanged(bool bChanged) {m_bChanged = bChanged;}
};

union UValue
{
    int m_iValue;
    double m_dValue;
    bool operator==(UValue right) {return m_iValue == right.m_iValue;}
};

class CConstant: public CUnit
{
public:
    enum eCONST {INTEGER, REAL};
private:
    eCONST m_type;
    GNode *m_gnType;
    string m_strTreeType;
    UValue m_uValue;
    unsigned int m_uiNo;
public:
    CConstant(eCONST type = INTEGER): m_type(type) {SetUnit(CONST);}
    unsigned int GetNo() {return m_uiNo;}
    void SetNo(unsigned int uiNo) {m_uiNo = uiNo;}
    GNode* GetType() {return m_gnType;}
    void SetType(GNode *gnType) {m_gnType = gnType;}
    UValue GetValue() {return m_uValue;}
    void SetValue(UValue uValue) {m_uValue = uValue;}
    string GetTreeType() {return m_strTreeType;}
    void SetTreeType(string strTreeType) {m_strTreeType = strTreeType;}
    void Print();
    bool operator==(CUnit &right)
    {
	return (m_gnType == ((CConstant*) (&right))->m_gnType)? false: ((m_uValue == ((CConstant*)(&right))->m_uValue)? true: false);
    }
#if 0
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
#endif
};

#if 0
class CExprUnit
{
private:
    CConstant m_const;
    CVar m_var;
    bool m_bVar;
public:
    bool IsVar() {return m_bVar;}
    bool IsConst() {return !m_bVar;}
    CExprUnit(void* pExprUnit, bool bVar = true)
    {
	if(bVar)
	{
	    m_var = *(CVar*) pExprUnit;
	    m_var.SetUnit(CUnit::VAR);
	}
	else
	{
	    m_const = *(CConstant*) pExprUnit;
	    m_var.SetUnit(CUnit::CONST);
	}
    }
};
#endif

struct SOp
{
    string m_strOp;
    unsigned int m_uiNArgs;
    SOp& operator=(SOp right) {m_strOp = right.m_strOp; m_uiNArgs = right.m_uiNArgs;}
    SOp(string strOp = "", unsigned int uiNArgs = 0): m_strOp(strOp), m_uiNArgs(uiNArgs) {}
    bool operator==(SOp right) {return (m_strOp != right.m_strOp)? false: (m_uiNArgs == right.m_uiNArgs)? true: false;}
    bool operator!=(SOp right) {return !(*this == right);}
};

class CExpr: public CUnit
{
private:
    SOp m_op;
public:
    CExpr() {SetUnit(EXPR);}
    bool IsEqualExpr(CExpr);
    bool IsCompleExpr(CExpr);
    void Print();
#if 0
    {
	for(int i = 0; i < m_op.m_uiNArgs; i++)
	{
	    ((CUnit*) m_operands[i])->Print();
	}
	cout << m_op.m_strOp << endl;
    }
#endif
    void AddOperand(CUnit* pOperand) {m_operands.push_back(pOperand);}
    void SetOp(SOp op) {m_op = op;}
    bool operator==(CUnit &right);
    vector<CVar*> CollectVars();
    CUnit* GetOperand(unsigned int uiOperand) {return m_operands[uiOperand];}
public:
    vector<CUnit*> m_operands;
    SOp GetOp() {return m_op;}
};

class CTesting: public CExpr
{
    
};

class CStmtList;
class CBind;
class CCondStmt;

class CStmt: public CExpr
{
private:
    CExpr *m_pStmt;
    int m_iLine;
public:
    CStmt() {SetUnit(STMT);}
    virtual void Print() = 0;
    CStmt* ConstructStmt0(GNode*);
    virtual CStmt* ConstructStmt(GNode*) = 0;
    int GetLine() {return m_iLine;}
    void SetLine(const int iLine) {m_iLine = iLine;}
    virtual vector<CVar*> AnalyzeChangedVar() = 0;
};

class CStmtList: public CUnit
{
private:
    vector<CStmt*> m_vecStmt;
public:
#if 0
    CStmtList& operator=(CStmtList &right)
    {
	m_vecStmt = right.m_vecStmt;
	return *this;
    }
#endif
    CStmtList() {SetUnit(STMTLIST); m_vecStmt.clear();}
    void Print();
    void ConstructStmtList(GNode*);
    CStmtList& operator=(CStmtList &right)
    {
	for(int i = 0; i < right.m_vecStmt.size(); i++)
	    m_vecStmt.push_back(right.m_vecStmt[i]);
	return *this;
    }
    bool operator==(CUnit &right) {return true;}
    void AddStmt(CStmt* pStmt) {m_vecStmt.push_back(pStmt);}
    unsigned int Size() {return m_vecStmt.size();}
    CStmt* GetOperand(unsigned int uiOperand) 
    {
	if(uiOperand < 0 || uiOperand >= m_vecStmt.size())
	    return 0;
	return m_vecStmt[uiOperand];
    }
};

class CBind: public CUnit
{
private:
    vector<CVar*> m_vecVar;
public:
    CStmtList m_stmtList;
private:
    vector<CVar*> m_vecChanged;
public:
    CBind() {SetUnit(BIND);}
    void Print();
    CBind* ConstructBind(GNode*);
    bool operator==(CUnit &right) {return true;}
    void ConstructVars(GNode*);
    vector<CVar*> AnalyzeChangedVar();
    bool IsEmpty()
    {
	m_stmtList.Size();
    }
    vector<CVar*> GetChangedVar() {return m_vecChanged;}
    CStmtList GetStmtList() {return m_stmtList;}
};

class CCondStmt: public CStmt
{
public:
    CCondStmt() {SetUnit(CONDSTMT);}
    enum eRELOP {RELOP_LE, RELOP_LT, RELOP_EQ, RELOP_NE, RELOP_GE, RELOP_GT};
    void Print();
    CStmt* ConstructStmt(GNode*);
    CExpr* ConstructTesting(GNode*);
    vector<CVar*> AnalyzeChangedVar();
    CExpr* GetTesting() {return m_testing;}
    // condition statement which has no else part, like
    // if(**)
    // {
    //	    ...
    //	    ...
    // }
    bool HasNoElse()
    {
	return m_else.IsEmpty();
    }
    bool IsTestingVarChangedInThen();
private:
    CTesting *m_testing;
    CBind m_then;
    CBind m_else;
    vector<CVar*> m_testingVars;
};

class CModifyStmt: public CStmt
{
public:
    CModifyStmt() {SetUnit(MODIFYSTMT);}
    void Print();
    CStmt* ConstructStmt(GNode*);
    vector<CVar*> AnalyzeChangedVar() 
    {
	vector<CVar*> vec_var;
	vec_var.push_back(m_left);
	return vec_var;
    }
private:
    CVar *m_left;
    CExpr *m_right;
};

class CPreIncreStmt: public CStmt
{
public:
    CPreIncreStmt() {SetUnit(PREINCRESTMT);}
    void Print();
    CStmt* ConstructStmt(GNode*);
    vector<CVar*> AnalyzeChangedVar()
    {
	vector<CVar*> vec_var;
	vec_var.push_back(m_pVar);
	return vec_var;
    }
private:
    CVar *m_pVar;
};

class CPostIncreStmt: public CStmt
{
public:
    CPostIncreStmt() {SetUnit(POSTINCRESTMT);}
    void Print();
    CStmt* ConstructStmt(GNode*);
    vector<CVar*> AnalyzeChangedVar()
    {
	vector<CVar*> vec_var;
	vec_var.push_back(m_pVar);
	return vec_var;
    }
private:
    CVar *m_pVar;
};

class CPreDecreStmt: public CStmt
{
public:
    CPreDecreStmt() {SetUnit(PREDECRESTMT);}
    void Print();
    CStmt* ConstructStmt(GNode*);
    vector<CVar*> AnalyzeChangedVar()
    {
	vector<CVar*> vec_var;
	vec_var.push_back(m_pVar);
	return vec_var;
    }
private:
    CVar *m_pVar;
};

class CPostDecreStmt: public CStmt
{
public:
    CPostDecreStmt() {SetUnit(POSTDECRESTMT);}
    void Print();
    CStmt* ConstructStmt(GNode*);
    vector<CVar*> AnalyzeChangedVar()
    {
	vector<CVar*> vec_var;
	vec_var.push_back(m_pVar);
	return vec_var;
    }
private:
    CVar *m_pVar;
};

class CCleanupStmt: public CStmt
{
public:
    static CStmt* ConstructCleanupStmt(GNode*);
};

void PrintVectorVar(vector<CVar*>);

#endif
