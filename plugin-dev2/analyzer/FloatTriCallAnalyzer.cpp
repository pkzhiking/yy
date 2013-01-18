
#include "include/FloatTriCallAnalyzer.h"
#include "../core/include/ASTTreeRebuild.h"
#include "../core/include/TreePathWalker.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include "../core/include/FunctionIndex.h"
#include "../util/include/Util.h"
#include <sstream>
using std::stringstream;

FloatTriCallAnalyzer::FloatTriCallAnalyzer()
{
    setAnalyzerNodeType("call_expr");
}
FloatTriCallAnalyzer::~FloatTriCallAnalyzer()
{

}
    void
FloatTriCallAnalyzer::analyzeNode(GNode* node,const vector<int>& context)
{
    string calledFunction = NodeProcessor::getFunctionCallName(node);
    if(0 == calledFunction.compare("sin") || 0 == calledFunction.compare("cos") || 0 == calledFunction.compare("tan")){
        GNode* node1 = NodeProcessor::getFieldNode(node, "0");
        GNode* node2 = NodeProcessor::getOperand(node1, 0);
        GNode* node3 = NodeProcessor::getFieldNode(node2, "type");
        GNode* node4 = NodeProcessor::getFieldNode(node3, "size");
        string str = NodeProcessor::getFieldStr(node4, "low");
        int paraPrec = Util::stringToInt(str);
        if(paraPrec == 32){
            Logger::a("FloatTriCallAnalyzer") \
                <<"The trigonometric function" << " " + calledFunction + " "  << "can be replaced with a float one like sinf for the parameter is a float type variable" \
                <<" "<<SrcManager::getInstance().getFullFileName() \
                <<":"<<ContextProcessor::inferSrcPositon(context) <<" " \
                <<SrcManager::getInstance().getLine(ContextProcessor::inferSrcPositon(context))<<endl;

            stringstream reportMsgStream;
            reportMsgStream<<"FloatTriCallAnalyzer: replace the trigonometric function with a float one" <<  endl;
            string reportMsg = reportMsgStream.str();
            ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),ContextProcessor::inferSrcPositon(context),reportMsg);
            ProblemList::GetInstance().Add((char*)"FloatTriCallAnalyzer", SrcManager::getInstance().getFullFileName(), ContextProcessor::inferSrcPositon(context));
        }
    }
}

    void
FloatTriCallAnalyzer::startAnalyze()
{

}
    void
FloatTriCallAnalyzer::finishAnalyze()
{

}

    void
FloatTriCallAnalyzer::clearAnalyzerState()
{

}
