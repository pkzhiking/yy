#include "include/IfToIfelseAnalyzer.h"
#include "../core/include/Expr.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../util/include/Util.h"

IfToIfelseAnalyzer::IfToIfelseAnalyzer()
{
    setAnalyzerNodeType("bind_expr");
    //setAnalyzerNodeType("gt_expr");
    //setAnalyzerNodeType("var_decl");
    //setAnalyzerNodeType("parm_decl");
    //setAnalyzerNodeType("field_decl");
    
    //setAnalyzerNodeType("integer_type");
    //setAnalyzerNodeType("integer_cst");
    //setAnalyzerNodeType("real_cst");
    
}

IfToIfelseAnalyzer::~IfToIfelseAnalyzer()
{
}

void IfToIfelseAnalyzer::startAnalyze()
{
}


void IfToIfelseAnalyzer::finishAnalyze()
{
    if(mConditionNodeVector.size() < 2)
	return ;
    vector<int> conSet;
    conSet.clear();
    conSet.push_back(0);
    vector<conditionNode>::iterator iter = mConditionNodeVector.begin();

    for(int id = 1; iter != mConditionNodeVector.end(); iter++, id++)
    {
	if(iter->varId == (iter -1)->varId)
	{
	    conSet.push_back(id);
	}
	else
	{
	    isIfToIfelse(conSet);
	    conSet.clear();
	    conSet.push_back(id);
	}
    }
    isIfToIfelse(conSet);
}

void IfToIfelseAnalyzer::analyzeNode(GNode *node,const vector<int> &context)
{
    if(NodeProcessor::isBindExpr(node))
    {
	//int iNextBindIndex = NodeProcessor::NextBindIndex(node);
	//cout << "Next Bind Node Index:" << iNextBindIndex << endl;
	CBind bind;
	bind.ConstructBind(node);
	//cout << "BIND" << endl;
	bind.Print();
#if 0
	CStmtList stmtList = bind.GetStmtList();
	for(int i = 1; i < stmtList.Size(); i++)
	{
	    CStmt *pLastCondStmt = stmtList.GetOperand(i - 1);
	    CStmt *pCurCondStmt = stmtList.GetOperand(i);
	    if(CUnit::CONDSTMT == pLastCondStmt->GetUnit() && CUnit::CONDSTMT == pCurCondStmt->GetUnit() &&!((CCondStmt*) pLastCondStmt)->IsTestingVarChangedInThen())
	    {

		CExpr *pLastTesting = ((CCondStmt*) pLastCondStmt)->GetTesting();
		CExpr *pCurTesting = ((CCondStmt*) pCurCondStmt)->GetTesting();
		if("truth_andif_expr" == pCurTesting->GetOp().m_strOp || "truth_orif_expr" == pCurTesting->GetOp().m_strOp)
		{
		    cout << "FOUND IT" << endl;
		    CExpr *pOperand0 = (CExpr*) pCurTesting->GetOperand(0);
		    CExpr *pOperand1 = (CExpr*) pCurTesting->GetOperand(1);
		    if((*pOperand0 == *pLastTesting || *pOperand1 == *pLastTesting))
			log(pLastCondStmt->GetLine());
		}
	    }
	}
#endif
    }
#if 0
    else
    {
    GNode *pGnTesting = NodeProcessor::getOperand(node, 0);
    CUnit *pExpr = NodeProcessor::ConstructExpr(pGnTesting);
    pExpr->Print();
    string strTreeType = node->getTreeCode();
    if(NodeProcessor::IsSimpleExpr(node))
    {
	void *pSimpleExpr = NodeProcessor::GetSimpleExpr(node);
	if(NodeProcessor::isVariable(node))
	{
	    cout << "It's a variable" << endl;
	    ((CVar*)pSimpleExpr)->Print();
	}
	else
	{
	    if(NodeProcessor::IsConst(node))
	    {
		cout << "It's a const" << endl;
		((CConstant*)pSimpleExpr)->Print();
	    }
	}
    }
    NodeProcessor::PrintTreeCode();
    GTree *gTree = node->_getTree();
    gTree->Print();
    int nArgs = NodeProcessor::GetNArgs(node);
    cout << "NArgs:" << nArgs << endl;
    node->Print();
    cout << endl;
    CConstant *pConstant = NodeProcessor::ConstructConst(node);
    if(pConstant)
	pConstant->Print();
    CVar *pVar;
    pVar = NodeProcessor::ConstructVar(node);
    if(pVar)
	pVar->Print();
    NodeProcessor::GetName(node);
    CExpr *expr = NodeProcessor::GetExpr(node);
    conditionNode conNode;
    conNode.varId = NodeProcessor::getConditionVarID(node);

    if(-1 == conNode.varId)
    {
	return ;
    }

    conNode.node = node;
    if(NULL != node)
	conNode.iContextLine = Util::stringToInt(node->getProperty("line")->mStringProperty);
    conNode.testing = NodeProcessor::getCondTesting(node);

    conNode.father = (context.size() >= 1)? context[context.size() -1] : -1;
    conNode.testing = NodeProcessor::getCondTesting(node);
    mConditionNodeVector.push_back(conNode);
    }
#endif
}

void IfToIfelseAnalyzer::clearAnalyzerState()
{
    mConditionNodeVector.clear();
    mContextArray.clear();
}

void IfToIfelseAnalyzer::isIfToIfelse(const vector<int> &conSet)
{
    if(conSet.size() < 2)
	return ;

    for(int id = 1; id < conSet.size(); id++)
    {
	if(mConditionNodeVector[conSet[id]].father == mConditionNodeVector[conSet[id - 1]].father)
	{
	    if(mConditionNodeVector[conSet[id]].testing.find(mConditionNodeVector[conSet[id -1]].testing) != string::npos)
	    {
		log(mConditionNodeVector[conSet[id -1]].iContextLine);
	    }
	}
    }
}

void IfToIfelseAnalyzer::log(int lineNum)
{
    Logger::a("IfToIfelseAnalyzer") << "if statement list can be changed to ifelse" \
	<< SrcManager::getInstance().getFullFileName() << ":" << lineNum \
	<< SrcManager::getInstance().getLine(lineNum) << endl;
    stringstream reportMsgStream;
    reportMsgStream << "IfToIfelseAnalyzer: if statement list can be changed to ifelse" << endl;
    string reportMsg = reportMsgStream.str();
    ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(), lineNum, reportMsg);
    ProblemList::GetInstance().Add((char*) "IfToIfelseAnalyzer", SrcManager::getInstance().getFullFileName(), lineNum);
}
