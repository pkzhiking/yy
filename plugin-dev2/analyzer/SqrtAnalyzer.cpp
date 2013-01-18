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
#include <sstream>
using std::stringstream;
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
	if ( 0 == srcpStr.compare( 0, 11, "math.h" ) ){
		GNode *funcNameNode = NodeProcessor::getFieldNode( node, "name" );
		string funcNameStr = NodeProcessor::getFieldStr( funcNameNode, "strg" );
		if ( 0 == funcNameStr.compare( "sqrt" ) ){
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
}

void SqrtAnalyzer::startAnalyze(){
}

void SqrtAnalyzer::finishAnalyze(){
}
