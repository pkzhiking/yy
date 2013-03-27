/*
 * FunctionParameterAnalyzer.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: kiddlee
 *
 */

#include "include/FunctionParameterAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"

FunctionParameterAnalyzer::FunctionParameterAnalyzer() {
    setAnalyzerNodeType("parm_decl");
    setAnalyzerNodeType("function_decl");
    threshold = 4;
    fun_decl_line_num = 0;
}

FunctionParameterAnalyzer::~FunctionParameterAnalyzer() {

}

void FunctionParameterAnalyzer::analyzeNode(GNode *node,
                                            const vector<int> &context) {
    function_name = globalASTTreePointer->_getFunctionName();
    GNode *scopeNode = NodeProcessor::getFieldNode(node, "scpe");
    if(scopeNode){
        if(NodeProcessor::isFunctionDecl(node)){
            string tmp_str = NodeProcessor::getFieldStr(node, "srcp");
            size_t pos = tmp_str.find_last_of(":");
           fun_decl_line_num = Util::stringToInt(tmp_str.substr(pos+1));
        }
        else {
            if(0==FunctionCallNodeProcessor::getFunctionName(scopeNode).compare(function_name)) {
                string param_name = FunctionCallNodeProcessor::getParameterName(node);
                //cout << "fun:" << function_name << " para:" << param_name << endl; 
                params.insert(param_name);
            }
        }
    }
}

void FunctionParameterAnalyzer::startAnalyze() {
}

void FunctionParameterAnalyzer::finishAnalyze() { 
    //cout << "finish function analyze:" << function_name << endl;
    //cout << "params size:" << params.size() << endl;
    if (params.size() >= threshold) {
        Logger::a("FunctionParameterAnalyzer") << "Function: " << function_name
            << " has " << params.size()
            << " parameters, it's better to reduce the number of function parameters "
            << SrcManager::getInstance().getFullFileName() << ":"
            << fun_decl_line_num << " "
            << SrcManager::getInstance().getLine(fun_decl_line_num) << endl;

        stringstream reportMsgStream;
        reportMsgStream<<"FunctionParameterAnalyzer:"<< "Function: " << function_name
            << " has " << params.size()
            << " parameters, it's better to reduce the number of function parameters. " <<  endl;
        string reportMsg = reportMsgStream.str();
        ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),fun_decl_line_num,reportMsg);
        ProblemList::GetInstance().Add((char*)"FunctionParameterAnalyzer", SrcManager::getInstance().getFullFileName(), fun_decl_line_num);
    }
}
void FunctionParameterAnalyzer::clearAnalyzerState(){
    //cout << "clear function:" << function_name << endl;
    function_name = "";
    fun_decl_line_num = 0;
    params.clear();
}

