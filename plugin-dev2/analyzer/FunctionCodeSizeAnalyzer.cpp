/*
 * FunctionCodeSizeAnalyzer.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: kiddlee
 */

#include "include/FunctionCodeSizeAnalyzer.h"
#include "../util/include/Logger.h"
#include "../util/include/Util.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ContextProcessor.h"
#include "../core/include/SrcManager.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"

FunctionCodeSizeAnalyzer::FunctionCodeSizeAnalyzer() {
	setAnalyzerNodeType("return_expr");
	setAnalyzerNodeType("function_decl");
	setAnalyzerNodeType("expr_stmt");
	setAnalyzerNodeType("bind_stmt");
	setAnalyzerNodeType("cond_expr");
	setAnalyzerNodeType("for_stmt");
	setAnalyzerNodeType("do_stmt");
	setAnalyzerNodeType("while_stmt");
	codeSize = 0;
	threshold = 4;
	printed = false;
	fun_decl_line_num = 0;
}

FunctionCodeSizeAnalyzer::~FunctionCodeSizeAnalyzer() {

}

void FunctionCodeSizeAnalyzer::analyzeNode(GNode *node,
		const vector<int> &context) {
	if (NodeProcessor::isFunctionDecl(node)) {
		string tmp_str = NodeProcessor::getFieldStr(node, "srcp");
		size_t pos = tmp_str.find_last_of(":");
		function_name = FunctionCallNodeProcessor::getFunctionName(node);
		fun_decl_line_num = Util::stringToInt(tmp_str.substr(pos + 1));
	}

	codeSize++;
}

void FunctionCodeSizeAnalyzer::startAnalyze() {
}

void FunctionCodeSizeAnalyzer::finishAnalyze() {

	if (codeSize < threshold && false == printed) {
		Logger::a("FunctionCodeSizeAnalyzer") << "Function: " << function_name
				<< " has only " << codeSize
				<< " code lines, it's better to replace it with a macro. "
				<< SrcManager::getInstance().getFullFileName() << ":"
				<< fun_decl_line_num << " "
				<< SrcManager::getInstance().getLine(fun_decl_line_num) << endl;

		stringstream reportMsgStream;
							reportMsgStream<<"FunctionCodeSizeAnalyzer:"<< "Function: " << function_name
									<< " has only " << codeSize
									<< " code lines, it's better to replace it with a macro. " <<  endl;
							string reportMsg = reportMsgStream.str();
							ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),fun_decl_line_num,reportMsg);
							ProblemList::GetInstance().Add((char*)"FunctionCodeSizeAnalyzer", SrcManager::getInstance().getFullFileName(), fun_decl_line_num);

		printed = true;
	}
}
void FunctionCodeSizeAnalyzer::clearAnalyzerState(){
	codeSize = 0;
	threshold = 4;
	printed = false;
	fun_decl_line_num = 0;
}

