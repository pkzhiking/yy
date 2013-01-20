/*
 *GlobalVarSizeAnalyzer.cpp
 */
#include "GlobalVarSizeAnalyzer.h"
#include "Logger.h"
#include "NodeProcessor.h"
#include "Util.h"
#include "SrcManager.h"
#include "ContextProcessor.h"

#include <iostream>
using std::endl;

GlobalVarSizeAnalyzer::GlobalVarSizeAnalyzer(){
	setAnalyzerNodeType( "var_decl" );
        totalGlobalVarSize = 0;
}

GlobalVarSizeAnalyzer::~GlobalVarSizeAnalyzer(){
	globalVarMap.clear();
}

void GlobalVarSizeAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
	GNode *scpeNode = NodeProcessor::getFieldNode( node, "scpe" );
	if ( NodeProcessor::isGlobalScope( scpeNode ) ){
		GNode *sizeNode = NodeProcessor::getFieldNode( node, "size" );
		if ( NodeProcessor::ifIntConstant( sizeNode ) ){/*should be a integer_cst*/
			int size = Util::stringToInt( NodeProcessor::getFieldStr( sizeNode, "low" ) );
			string name = NodeProcessor::getFieldStr( NodeProcessor::getFieldNode( node, "name" ), "strg");
			std::map<string, int>::iterator itr = globalVarMap.find( name );
			if ( itr == globalVarMap.end() ){/*not find*/
				totalGlobalVarSize += size;
				globalVarMap.insert( std::map< string, int >::value_type( name, size ) );
			} else{
				int orgSize = itr->second;
				if ( orgSize < size ){
					totalGlobalVarSize += size - orgSize;
					itr->second = orgSize;
				}
			}
		}
	}
}

void GlobalVarSizeAnalyzer::startAnalyze(){
}

void GlobalVarSizeAnalyzer::finishAnalyze(){
	        Logger::a( "GlobalVarSizeAnalyzer" ) \
                << "the total size of global variables till now is " << totalGlobalVarSize << endl;
}
