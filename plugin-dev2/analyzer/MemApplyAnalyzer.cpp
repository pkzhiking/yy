
#include "include/MemApplyAnalyzer.h"
#include "../core/include/ASTTreeRebuild.h"
#include "../core/include/TreePathWalker.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
MemApplyAnalyzer::MemApplyAnalyzer(){
	setAnalyzerNodeType("call_expr");
	memApplyCallCounter = 0;
	codeLocation = -1;
}

MemApplyAnalyzer::~MemApplyAnalyzer(){
}
void
MemApplyAnalyzer::clearAnalyzerState() {
	memApplyCallCounter = 0;
	codeLocation = -1;
}
void
MemApplyAnalyzer::analyzeNode(GNode* node,const vector<int>& context){

	if (NodeProcessor::isMemApplyFunction(node)){

		int loopCounter = NodeProcessor::isInLoop(context);

		if (loopCounter)	{

			//MemApplyAnalyzer::memApplyCallCounter += (8 * (1 << (loopCounter - 1)));
			Logger::a("MemApplyAnalyzer") \
					<<"MemApplyCall find in loop counter " \
					<<loopCounter<<" "<<SrcManager::getInstance().getFullFileName() \
					<<":"<<ContextProcessor::inferSrcPositon(context) <<" " \
					<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

			stringstream reportMsgStream;
						reportMsgStream<<"MemApplyAnalyzer: MemApplyCall find in loop counter "<<endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
						ProblemList::GetInstance().Add((char*)"MemApplyAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
		}else{
			int location = ContextProcessor::inferSrcPositon(context);
			if (location != -1 && codeLocation == -1){
				codeLocation = location;
			}
			MemApplyAnalyzer::memApplyCallCounter++;
		}
  }
}

void
MemApplyAnalyzer::startAnalyze(){
}

void
MemApplyAnalyzer::finishAnalyze(){

	if (MemApplyAnalyzer::memApplyCallCounter > 5){

		Logger::a("MemApplyAnalyzer") \
				<<"Total MemApplyCall find in function " \
				<<MemApplyAnalyzer::memApplyCallCounter<<" times! "<< codeLocation << endl;

		stringstream reportMsgStream;
						reportMsgStream<<"MemApplyAnalyzer: Total MemApplyCall find in function "
								<<MemApplyAnalyzer::memApplyCallCounter<<" times! " << endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),codeLocation,reportMsg);

		MemApplyAnalyzer::memApplyCallCounter = 0;

	}
}



