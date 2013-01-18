/*
 *BitNotExprAnalyzer.cpp
 */
#include "include/BitNotExprAnalyzer.h"
#include "../util/include/Logger.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
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

	stringstream reportMsgStream;
					reportMsgStream<<"BitNotExprAnalyzer: Bit not expression found (may result in unwanted result) "<<endl;
					string reportMsg = reportMsgStream.str();
					ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
					ProblemList::GetInstance().Add((char*)"BitNotExprAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

}

void BitNotExprAnalyzer::startAnalyze(){
}

void BitNotExprAnalyzer::finishAnalyze(){
}
