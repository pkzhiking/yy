/*
 *GlobalVarSizeAnalyzer.cpp
 */
#include "include/GlobalVarSizeAnalyzer.h"
#include "../util/include/Logger.h"
#include "../core/include/NodeProcessor.h"
#include "../util/include/Util.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <iostream>
using std::endl;

GlobalVarSizeAnalyzer::GlobalVarSizeAnalyzer(){
    setAnalyzerNodeType( "var_decl" );
    totalGlobalVarSize = 0;
    codeLocation = 0;
}

GlobalVarSizeAnalyzer::~GlobalVarSizeAnalyzer(){
    totalGlobalVarSize = 0;
    codeLocation = 0;
    globalVarMap.clear();
}

void GlobalVarSizeAnalyzer::analyzeNode( GNode *node, const vector< int > &context ){
    GNode *scpeNode = NodeProcessor::getFieldNode( node, "scpe" );

    if (codeLocation == 0){
        int varLocation = ContextProcessor::inferSrcPositon(context);
        if (varLocation != -1){
            codeLocation = varLocation;
        }
    }
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
    if(totalGlobalVarSize >= 4096) {
        Logger::a( "GlobalVarSizeAnalyzer" ) \
            << "the total size of global variables till now is " << totalGlobalVarSize << endl;

        if (codeLocation == 0)
            return ;
        stringstream reportMsgStream;
        reportMsgStream<<"GlobalVarSizeAnalyzer : the total size of global variables till now is " << totalGlobalVarSize <<endl;
        string reportMsg = reportMsgStream.str();
        ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),codeLocation,reportMsg);
        ProblemList::GetInstance().Add((char*)"GlobalVarSizeAnalyzer", SrcManager::getInstance().getFullFileName(), codeLocation);
    }
}

void GlobalVarSizeAnalyzer::clearAnalyzerState(){
    totalGlobalVarSize = 0;
    codeLocation = 0;
    globalVarMap.clear();
}
