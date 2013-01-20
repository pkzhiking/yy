/*
 *SqrtAnalyzer.cpp
 */
#include "SqrtAnalyzer.h"
#include "Logger.h"
#include "SrcManager.h"
#include "ContextProcessor.h"
#include "NodeProcessor.h"

#include <iostream>
#include <string>
using std::endl;

SqrtAnalyzer::SqrtAnalyzer(){
	setAnalyzerNodeType( "function_decl" );
}

SqrtAnalyzer::~SqrtAnalyzer(){
}

void SqrtAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	string srcpStr = NodeProcessor::getFieldStr( node, "srcp" );
	if ( 0 == srcpStr.compare( 0, 11, "mathcalls.h" ) ){
		GNode *funcNameNode = NodeProcessor::getFieldNode( node, "name" );
		string funcNameStr = NodeProcessor::getFieldStr( funcNameNode, "strg" );
		if ( 0 == funcNameStr.compare( "sqrt" ) ){
			Logger::a( "SqrtAnalyzer" ) \
				<< "sqrt call to math.h found " \
				<< SrcManager::getInstance().getFullFileName() \
				<< ":" << ContextProcessor::inferSrcPositon( context ) << " " \
				<< SrcManager::getInstance().getLine( ContextProcessor::inferSrcPositon( context ) ) << endl;
		}
	}
}

void SqrtAnalyzer::startAnalyze(){
}

void SqrtAnalyzer::finishAnalyze(){
}
