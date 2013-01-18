/*
 *ForLoopToZeroAnalyzer.cpp
 */
#include "include/ForLoopToZeroAnalyzer.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../util/include/Util.h"
#include "../core/include/SrcManager.h"
#include "../util/include/Logger.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
#include <iostream>
using std::endl;

ForLoopToZeroAnalyzer::ForLoopToZeroAnalyzer(){
	setAnalyzerNodeType( "for_stmt" );
	condVarVec.clear();
	iterVarVec.clear();
}
void
ForLoopToZeroAnalyzer::clearAnalyzerState(){
	condVarVec.clear();
	iterVarVec.clear();
}

ForLoopToZeroAnalyzer::~ForLoopToZeroAnalyzer(){
}

void ForLoopToZeroAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	findVarInCond( NodeProcessor::getFieldNode( node, "cond" ) );	
	findIterWithExpr( NodeProcessor::getFieldNode( node, "expr" ) );
	/*check for initialization*/
	stmtNode = node;
	checkInitStmt( ContextProcessor::getPreStmt( context, node->getIndex() ), context );
	condVarVec.clear();
	iterVarVec.clear();
}

void ForLoopToZeroAnalyzer::startAnalyze(){
}

void ForLoopToZeroAnalyzer::finishAnalyze(){
}

void ForLoopToZeroAnalyzer::findVarInCond( GNode *node ){
	if ( NodeProcessor::isAndOrCondNode( node ) ){/*complex condition with && or ||*/
		findVarInCond( NodeProcessor::getOperand( node, 0 ) );
		findVarInCond( NodeProcessor::getOperand( node, 1 ) );
	} else if ( NodeProcessor::isCompCondNode( node ) ){/*normal compare node*/
		GNode *op0 = NodeProcessor::getOperand( node, 0 ),
			*op1 = NodeProcessor::getOperand( node, 1 );
		if ( NodeProcessor::isVariable( op0 ) ){
			string nameStr = NodeProcessor::getFieldStr( NodeProcessor::getFieldNode( op0, "name" ), "strg");
			if ( !isStrInVector( nameStr, condVarVec ) )
				condVarVec.push_back( nameStr );
		}
		if ( NodeProcessor::isVariable( op1 ) ){
			string nameStr = NodeProcessor::getFieldStr( NodeProcessor::getFieldNode( op1, "name" ), "strg");
                        if ( !isStrInVector( nameStr, condVarVec ) )
                                condVarVec.push_back( nameStr );
		}
	}
}

void ForLoopToZeroAnalyzer::findIterWithExpr( GNode *node ){
	if ( NodeProcessor::isCleanupExpr( node ) || NodeProcessor::isConvertExpr( node ) )
		findIterWithExpr( NodeProcessor::getOperand( node, 0 ) );
	else if ( NodeProcessor::isCompoundExpr( node ) ){
		findIterWithExpr( NodeProcessor::getOperand( node, 0 ) );
		findIterWithExpr( NodeProcessor::getOperand( node, 1 ) );
	} else if ( NodeProcessor::isModifyStmt( node ) ){
		GNode *op0 = NodeProcessor::getOperand( node, 0 );
		if ( NodeProcessor::isVariable( op0 ) ){
			string nameStr = NodeProcessor::getFieldStr( NodeProcessor::getFieldNode( op0, "name" ), "strg" );
			if ( !isStrInVector( nameStr, iterVarVec ) && isStrInVector( nameStr, condVarVec ) )
				iterVarVec.push_back( nameStr );
		}
	}
}

void ForLoopToZeroAnalyzer::checkInitStmt( GNode *node, const vector< int > &context ){
	if ( NULL == node )
		return;
	if ( NodeProcessor::isCleanupExpr( node ) || NodeProcessor::isConvertExpr( node ) )
                checkInitStmt( NodeProcessor::getOperand( node, 0 ), context );
        else if ( NodeProcessor::isCompoundExpr( node ) ){
		checkInitStmt( NodeProcessor::getOperand( node, 0 ), context );
                checkInitStmt( NodeProcessor::getOperand( node, 1 ), context );
        } else if ( NodeProcessor::isExprStmt( node ) ){
		checkInitStmt( NodeProcessor::getFieldNode( node, "expr" ), context );
	} else if ( NodeProcessor::isModifyExpr( node ) ){
		GNode *op0 = NodeProcessor::getOperand( node, 0 );
		if ( !NodeProcessor::isVariable( op0 ) )
			return;
                string nameStr = NodeProcessor::getFieldStr( NodeProcessor::getFieldNode( op0, "name" ), "strg" );
		GNode *op1 = NodeProcessor::getOperand( node, 1 );
		if ( NodeProcessor::ifIntConstant( op1 ) )
			if ( 0 == Util::stringToInt(op1->getProperty("low")->mStringProperty) )
				 Logger::a( "ForLoopToZeroAnalyzer" ) \
					<< "Loop iterator is initiated to 0 (may be rewrite with a reverse loop to 0) " \
					<< SrcManager::getInstance().getFullFileName() \
					<< ":" << NodeProcessor::getFieldStr( stmtNode, "line" ) \
					<< " " << SrcManager::getInstance().getLine( Util::stringToInt( NodeProcessor::getFieldStr( stmtNode, "line" ) )  ) << endl;

				stringstream reportMsgStream;
						reportMsgStream<<"ForLoopToZeroAnalyzer: Loop iterator is initiated to 0 (may be rewrite with a reverse loop to 0) "<<endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),Util::stringToInt( NodeProcessor::getFieldStr( stmtNode, "line" ) ) ,reportMsg);	
						ProblemList::GetInstance().Add((char*)"ForLoopToZeroAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

	}
}

bool ForLoopToZeroAnalyzer::isStrInVector( string &str, vector< string > &vec ){
	vector<string>::iterator itr;
		for( itr = vec.begin(); itr!=vec.end(); itr++ )
			if ( 0 == itr->compare( str ) )
				return true;
	return false;
}

