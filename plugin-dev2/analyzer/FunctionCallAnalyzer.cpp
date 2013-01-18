
#include "include/FunctionCallAnalyzer.h"
#include "../core/include/ASTTreeRebuild.h"
#include "../core/include/TreePathWalker.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../core/include/FunctionIndex.h"
#include <sstream>
using std::stringstream;

FunctionCallAnalyzer::FunctionCallAnalyzer()
{
	setAnalyzerNodeType("call_expr");
}
FunctionCallAnalyzer::~FunctionCallAnalyzer()
{

}
void
FunctionCallAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
			string calledFunction = NodeProcessor::getCalledFunctionName(node);
			if (calledFunction == "")
			{
				return ;
			}
			ASTTreeRebuild *functionTree = new ASTTreeRebuild(FunctionIndex::getInstance().getLoction(calledFunction));

			if (functionTree->ASTTreePointer == NULL){
				return ;
			}

			if (isConditionFunction(functionTree->ASTTreePointer))
			{
				Logger::a("FunctionCallAnalyzer") \
								<<"The condition in function" << " " + calledFunction + " "  << "can be remove! " \
								<<" "<<SrcManager::getInstance().getFullFileName() \
								<<":"<<ContextProcessor::inferSrcPositon(context) <<" " \
								<<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

				stringstream reportMsgStream;
									reportMsgStream<<"FunctionCallAnalyzer: the condition in function can be early judged!" <<  endl;
									string reportMsg = reportMsgStream.str();
									ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
									ProblemList::GetInstance().Add((char*)"FunctionCallAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
			}

}
void
FunctionCallAnalyzer::startAnalyze()
{

}
void
FunctionCallAnalyzer::finishAnalyze()
{

}

void
FunctionCallAnalyzer::clearAnalyzerState()
{

}
bool FunctionCallAnalyzer::isConditionFunction(GTree* tree)
{

	 GNode * tNode = tree->findNodeByIndex(1);

	  while(tNode->getTreeCode() == "bind_expr"){
		  int id = tNode->getProperty("body")->mNodeProperty;
		  tNode = tree->findNodeByIndex(id);
	    }

	  if (tNode->getTreeCode() == "cond_expr"){
		  	  return true;
		  }

	 if (tNode->getTreeCode() == "statement_list"){

		  	vector < int > childs = tNode->getChilds();
		  	int id;
		  	for(id = 0; id < childs.size() - 1; id++){

		  		tNode = tree->findNodeByIndex(childs[id]);
		  		if (tNode->getTreeCode() != "decl_expr"){
		  			if (tNode->getTreeCode() == "cond_expr"){
		  				tNode = tree->findNodeByIndex(tNode->getProperty("op 1")->mNodeProperty);
							if (tNode->getTreeCode() == "return_expr")
								return true;
							else
								return false;
						}
		  			else
		  				return false;
					}
		  	  	}
		  	tNode = tree->findNodeByIndex(childs[id]);
		  	if (tNode->getTreeCode() == "cond_expr")
				{
		  			return true;
				}
	  }

  return false;
}

