/*
 *RealDivisionAnalyzer.cpp
 */
#include "RealDivisionAnalyzer.h"
#include "Logger.h"
#include "SrcManager.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"

#include <iostream>
using std::endl;

RealDivisionAnalyzer::RealDivisionAnalyzer(){
	setAnalyzerNodeType( "rdiv_expr" );
}

RealDivisionAnalyzer::~RealDivisionAnalyzer(){
}

void RealDivisionAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	GNode *op1 = NodeProcessor::getOperand( node, 1 );
	if ( NodeProcessor::isRealConstant( op1 ) ){ 
		Logger::a( "RealDivisionAnalyzer" ) \
			<< "Real divided by const found " \
			<< SrcManager::getInstance().getFullFileName() \
			<< ":" << ContextProcessor::inferSrcPositon( context ) << " " \
			<< SrcManager::getInstance().getLine( ContextProcessor::inferSrcPositon( context ) ) << endl;
	}
}

void RealDivisionAnalyzer::startAnalyze(){
}

void RealDivisionAnalyzer::finishAnalyze(){
}
