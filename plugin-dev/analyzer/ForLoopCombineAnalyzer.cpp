/*
 *ForLoopCombineAnalyzer.cpp
 */
#include "ForLoopCombineAnalyzer.h"
#include "Logger.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"
#include "SrcManager.h"
#include "Util.h"

#include <iostream>
using std::endl;

ForLoopCombineAnalyzer::ForLoopCombineAnalyzer(){
	setAnalyzerNodeType( "for_stmt" );
}

ForLoopCombineAnalyzer::~ForLoopCombineAnalyzer(){
}

void ForLoopCombineAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	GNode *preStmt = ContextProcessor::getPreStmt( context, node->getIndex() );
	if ( NULL == preStmt )
		return;
	preStmt = ContextProcessor::getPreStmt( context, preStmt->getIndex() );
	if ( NULL == preStmt )
		return;
	if ( NodeProcessor::isForStmt( preStmt ) )
		if ( checkTwoLoop( node, preStmt, context ) ){
			Logger::a( "ForLoopCombineAnalyzer" ) \
				<< "This for loop may be combine with previous for loop " \
				<< SrcManager::getInstance().getFullFileName() \
				<< ":" << NodeProcessor::getFieldStr( node, "line" ) \
				<< " " << SrcManager::getInstance().getLine( Util::stringToInt( NodeProcessor::getFieldStr( node, "line" ) )  ) << endl;
		} 
}

void ForLoopCombineAnalyzer::startAnalyze(){
}

void ForLoopCombineAnalyzer::finishAnalyze(){
}

bool ForLoopCombineAnalyzer::checkTwoLoop( GNode *node1, GNode *node2, const vector< int > &context ){
	if ( !checkTwoLoopCond( NodeProcessor::getFieldNode( node1, "cond"), NodeProcessor::getFieldNode( node2, "cond" ) ) )
		return false;
	if ( !checkTwoLoopExpr( NodeProcessor::getFieldNode( node1, "expr" ), NodeProcessor::getFieldNode( node2, "expr" ) ) )
		return false;
	if ( !checkTwoLoopInit( ContextProcessor::getPreStmt( context, node1->getIndex() ), ContextProcessor::getPreStmt( context, node2->getIndex() ) ) )
		return false;
	return true;
}

bool ForLoopCombineAnalyzer::checkTwoLoopCond( GNode *node1, GNode *node2 ){
	if ( !NodeProcessor::isTheSameType( node1, node2 ) )
		return false;
	if ( NodeProcessor::isAndOrCondNode( node1 ) )
		return checkTwoLoopCond( NodeProcessor::getOperand( node1, 0 ), NodeProcessor::getOperand( node2, 0 ) ) && checkTwoLoopCond( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) );
	if ( NodeProcessor::isCompCondNode( node1 ) )
		return ( NodeProcessor::getOperand( node1, 0 ) == NodeProcessor::getOperand( node2, 0 ) ) && ( NodeProcessor::getOperand( node1, 1 ) == NodeProcessor::getOperand( node2, 1 ) );
	return false;
}

bool ForLoopCombineAnalyzer::checkTwoLoopExpr( GNode *node1, GNode *node2 ){
        if ( !NodeProcessor::isTheSameType( node1, node2 ) )
                return false;
	if ( NodeProcessor::isCleanupExpr( node1 ) || NodeProcessor::isConvertExpr( node1 ) )
		return checkTwoLoopExpr( NodeProcessor::getOperand( node1, 0 ), NodeProcessor::getOperand( node2, 0 ) );
	if ( NodeProcessor::isCompoundExpr( node1 ) )
		return checkTwoLoopExpr( NodeProcessor::getOperand( node1, 0 ), NodeProcessor::getOperand( node2, 0 ) ) && checkTwoLoopExpr( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) ); 
	if ( NodeProcessor::isModifyStmt( node1 ) ){
		if ( NodeProcessor::isModifyExpr( node1 ) ) 
			return ( NodeProcessor::getOperand( node1, 0 ) == NodeProcessor::getOperand( node2, 0 ) ) && checkTwoExpression( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) );
		return ( NodeProcessor::getOperand( node1, 0 ) == NodeProcessor::getOperand( node2, 0 ) ) && ( NodeProcessor::getOperand( node1, 1 ) == NodeProcessor::getOperand( node2, 1 ) ); 
	}
	return false;
}

bool ForLoopCombineAnalyzer::checkTwoLoopInit( GNode *node1, GNode *node2 ){
	if ( node1 == NULL || node2 == NULL || !NodeProcessor::isTheSameType( node1, node2 ) )
		return false;
	if ( NodeProcessor::isCleanupExpr( node1 ) || NodeProcessor::isConvertExpr( node1 ) )
		return checkTwoLoopInit( NodeProcessor::getOperand( node1, 0 ), NodeProcessor::getOperand( node2, 0 ) );
	if ( NodeProcessor::isCompoundExpr( node1 ) )
		return checkTwoLoopExpr( NodeProcessor::getOperand( node1, 0 ), NodeProcessor::getOperand( node2, 0 ) ) && checkTwoLoopExpr( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) );
	if ( NodeProcessor::isExprStmt( node1 ) )
		return checkTwoLoopInit( NodeProcessor::getFieldNode( node1, "expr" ), NodeProcessor::getFieldNode( node2, "expr" ) );
	else if ( NodeProcessor::isModifyStmt( node1 ) ){
		if ( NodeProcessor::isModifyExpr( node1 ) )
 			return ( NodeProcessor::getOperand( node1, 0 ) == NodeProcessor::getOperand( node2, 0 ) ) && checkTwoExpression( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) );
		return ( NodeProcessor::getOperand( node1, 0 ) == NodeProcessor::getOperand( node2, 0 ) ) && ( NodeProcessor::getOperand( node1, 1 ) == NodeProcessor::getOperand( node2, 1 ) );
	}
	return false;
}

bool ForLoopCombineAnalyzer::checkTwoExpression( GNode *node1, GNode *node2 ){
	if ( node1 == node2 )
 		return true;
	if ( node1 == NULL || node2 == NULL )
		return false;
	if ( !NodeProcessor::isTheSameType( node1, node2 ) )
		return false;
	GNode *tNode1 = NodeProcessor::getOperand( node1, 0 ),
		*tNode2 = NodeProcessor::getOperand( node2, 0 );
	if ( tNode1 == NULL || tNode2 == NULL )
		return false;
	return checkTwoExpression( tNode1, tNode2 ) && checkTwoExpression( NodeProcessor::getOperand( node1, 1 ), NodeProcessor::getOperand( node2, 1 ) );
}
