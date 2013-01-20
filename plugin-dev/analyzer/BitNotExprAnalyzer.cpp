/*
 *BitNotExprAnalyzer.cpp
 */
#include "BitNotExprAnalyzer.h"
#include "Logger.h"
#include "SrcManager.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"

#include <iostream>
using std::endl;

BitNotExprAnalyzer::BitNotExprAnalyzer(){
	setAnalyzerNodeType( "bit_not_expr" );
}

BitNotExprAnalyzer::~BitNotExprAnalyzer(){
}

void BitNotExprAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	Logger::a( "BitNotExprAnalyzer" ) \
		<< "Bit not expression found (may result in unwanted result) " \
		<< SrcManager::getInstance().getFullFileName() \
		<< ":" << ContextProcessor::inferSrcPositon( context ) << " " \
		<< SrcManager::getInstance().getLine( ContextProcessor::inferSrcPositon( context ) ) << endl;
}

void BitNotExprAnalyzer::startAnalyze(){
}

void BitNotExprAnalyzer::finishAnalyze(){
}
