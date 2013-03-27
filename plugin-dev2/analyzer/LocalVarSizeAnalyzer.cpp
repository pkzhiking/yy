/*
 * LocalVarSizeAnalyzer.cpp
 *
 *  Created on: Aug 9, 2012
 *      Author: kiddlee
 */

#include "include/LocalVarSizeAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/FunctionCallNodeProcessor.h"
#include "../core/include/TotalProblem.h"

using std::endl;

LocalVarSizeAnalyzer::LocalVarSizeAnalyzer() {
    setAnalyzerNodeType("var_decl");
    setAnalyzerNodeType("function_decl");
    totalLocalVarSize = 0;
    //threshold = 4096;
}
void
LocalVarSizeAnalyzer::clearAnalyzerState() {
    totalLocalVarSize = 0;
    //threshold = 0;
}
LocalVarSizeAnalyzer::~LocalVarSizeAnalyzer() {

}

void LocalVarSizeAnalyzer::analyzeNode(GNode *node,
                                       const vector<int> &context) {
    if (NodeProcessor::isFunctionDecl(node)) {
        string tmp_str = NodeProcessor::getFieldStr(node, "srcp");
        size_t pos = tmp_str.find_last_of(":");
        function_name = FunctionCallNodeProcessor::getFunctionName(node);
        fun_decl_line_num = Util::stringToInt(tmp_str.substr(pos + 1));
    } else {
        GNode *scpeNode = NodeProcessor::getFieldNode(node, "scpe");
        if (!NodeProcessor::isGlobalScope(scpeNode)) {
            GNode *sizeNode = NodeProcessor::getFieldNode(node, "size");
            if (NodeProcessor::ifIntConstant(sizeNode)) {/*should be a integer_cst*/
                int size = Util::stringToInt(
                                             NodeProcessor::getFieldStr(sizeNode, "low"));
                totalLocalVarSize += size;
            }
        }
    }
}

void LocalVarSizeAnalyzer::startAnalyze() {
}

void LocalVarSizeAnalyzer::finishAnalyze() {
    if (totalLocalVarSize > 1024) {
        Logger::a("LocalVarSizeAnalyzer")
            << "The total size of local stack variables of function "
            << function_name << " is " << totalLocalVarSize << ". "
            << SrcManager::getInstance().getFullFileName() << ":"
            << fun_decl_line_num << " "
            << SrcManager::getInstance().getLine(fun_decl_line_num) << endl;

        stringstream reportMsgStream;
        reportMsgStream<<"LocalVarSizeAnalyzer:"<< "The total size of local stack variables of function "
            << function_name << " is " << totalLocalVarSize << ". "<<  endl;
        string reportMsg = reportMsgStream.str();
        ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),fun_decl_line_num,reportMsg);
        ProblemList::GetInstance().Add((char*)"LocalVarSizeAnalyzer", SrcManager::getInstance().getFullFileName(), fun_decl_line_num);
    }
}
