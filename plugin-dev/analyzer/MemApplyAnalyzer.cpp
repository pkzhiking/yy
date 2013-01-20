
#include "MemApplyAnalyzer.h"
MemApplyAnalyzer::MemApplyAnalyzer()
{
	setAnalyzerNodeType("call_expr");
	memApplyCallCounter = 0;
}
MemApplyAnalyzer::~MemApplyAnalyzer()
{

}
void
MemApplyAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
	if (NodeProcessor::isMemApplyFunction(node))
	{
		int loopCounter = NodeProcessor::isInLoop(context);
		if (loopCounter)
		{
			//MemApplyAnalyzer::memApplyCallCounter += (8 * (1 << (loopCounter - 1)));
			Logger::a("MemApplyAnalyzer") \
					<<"MemApplyCall find in loop counter " \
					<<loopCounter<<" "<<SrcManager::getInstance().getFullFileName() \
					<<":"<<ContextProcessor::inferSrcPositon(context) <<" " \
					<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;
		}
		else
		{
			MemApplyAnalyzer::memApplyCallCounter++;
		}
	}

}
void
MemApplyAnalyzer::startAnalyze()
{

}
void
MemApplyAnalyzer::finishAnalyze()
{
	if (MemApplyAnalyzer::memApplyCallCounter > 5)
	{
		Logger::a("MemApplyAnalyzer") \
				<<"Total MemApplyCall find in function " \
				<<MemApplyAnalyzer::memApplyCallCounter<<" times! "<< codeLocation << endl;
		MemApplyAnalyzer::memApplyCallCounter = 0;

	}
}

void
MemApplyAnalyzer::clearAnalyzerState()
{
	MemApplyAnalyzer::memApplyCallCounter = 0;
}


