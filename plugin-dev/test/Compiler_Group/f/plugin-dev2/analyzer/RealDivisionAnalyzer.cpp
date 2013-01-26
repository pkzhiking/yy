/*
 *RealDivisionAnalyzer.cpp
 */
#include "include/RealDivisionAnalyzer.h"
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

		stringstream reportMsgStream;
						reportMsgStream<<"RealDivisionAnalyzer: Real divided by const found "<<endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
						ProblemList::GetInstance().Add((char*)"RealDivisionAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));

	}
}

void RealDivisionAnalyzer::startAnalyze(){
}

void RealDivisionAnalyzer::finishAnalyze(){
}
