/*
 *SqrtAnalyzer.cpp
 */
#include "include/SqrtAnalyzer.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../core/include/ASTTree.h"
#include "../core/include/Expr.h"
#include "../util/include/Util.h"
#include <sstream>
using std::stringstream;
#include <iostream>
#include <string>
using std::endl;

SqrtAnalyzer::SqrtAnalyzer(){
	setAnalyzerNodeType( "call_expr" );
}

SqrtAnalyzer::~SqrtAnalyzer(){
}

void SqrtAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
    GTree *gtTree = node->_getTree();
    map<int, GNode*> mapNode = gtTree->_getNodeMap();
    unsigned int uiSize = mapNode.size();
    map<int, GNode*>::iterator iteMapNode = mapNode.begin();
    if(mapNode.end() != iteMapNode)
	iteMapNode++;
#if 0
    for(int i = 0; mapNode.end() != iteMapNode ; iteMapNode++, i++)
    {
	cout << "The " << i << "th key:" << iteMapNode->first << "; " << i << "th node address is:" << iteMapNode->second << " The size of mNodeMap:" << uiSize << endl;
    }
#endif
    string strFn = NodeProcessor::getFieldStr(node, "fn");
    strFn = strFn.erase(0, 1);
    
    unsigned int uiFuncAddrNode = Util::stringToInt(strFn);

    GNode *gnFuncAddrNode = mapNode[uiFuncAddrNode];

    GNode *gnFuncDeclNode = NodeProcessor::getOperand(gnFuncAddrNode, 0);

    string strSrcp = NodeProcessor::getFieldStr(gnFuncDeclNode, "srcp" );
    if ( 0 == strSrcp.compare( 0, 11, "mathcalls.h" ) )
    {
	string strNameNode = NodeProcessor::getFieldStr(gnFuncDeclNode, "name");
	strNameNode = strNameNode.erase(0, 1);
	unsigned int uiNameNode = Util::stringToInt(strNameNode);
	GNode *gnNameNode = mapNode[uiNameNode];
	string strFuncName = NodeProcessor::getFieldStr(gnNameNode, "strg");
	if (0 == strFuncName.compare(0, 4, "sqrt" ))
	{
	    cout << "Finally find a sqrt function at node" << endl;
	    gnNameNode->Print();
	    Logger::a( "SqrtAnalyzer" ) \
		<< "sqrt call to math.h found " \
		<< SrcManager::getInstance().getFullFileName() \
		<< ":" << ContextProcessor::inferSrcPositon( context ) << " " \
		<< SrcManager::getInstance().getLine( ContextProcessor::inferSrcPositon( context ) ) << endl;

	    stringstream reportMsgStream;
	    reportMsgStream<<"SqrtAnalyzer: sqrt call to math.h found  "<<endl;
	    string reportMsg = reportMsgStream.str();
	    ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
	    ProblemList::GetInstance().Add((char*)"SqrtAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
	}
    }
#if 0
			Logger::a( "SqrtAnalyzer" ) \
				<< "sqrt call to math.h found " \
				<< SrcManager::getInstance().getFullFileName() \
				<< ":" << ContextProcessor::inferSrcPositon( context ) << " " \
				<< SrcManager::getInstance().getLine( ContextProcessor::inferSrcPositon( context ) ) << endl;

			stringstream reportMsgStream;
							reportMsgStream<<"SqrtAnalyzer: sqrt call to math.h found  "<<endl;
							string reportMsg = reportMsgStream.str();
							ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
							ProblemList::GetInstance().Add((char*)"SqrtAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

		}
	}
#endif
}

void SqrtAnalyzer::startAnalyze(){
}

void SqrtAnalyzer::finishAnalyze(){
}
