#ifndef IFTOELSEANALYZER_H
#define IFTOELSEANALYZER_H

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
#include <string>
#include "../../core/include/ASTTree.h"

using std::vector;
using std::string;

class IfToIfelseAnalyzer : public BaseAnalyzer
{
public:
    struct conditionNode
    {
	int varId;
	int iContextLine;
	int father;
	GNode *node;
	string testing;
    };
    IfToIfelseAnalyzer();
    virtual ~IfToIfelseAnalyzer();
    virtual void analyzeNode(GNode*, const vector<int>&);
    virtual void startAnalyze();
    virtual void finishAnalyze();
    virtual void clearAnalyzerState();
private:
    void isIfToIfelse(const vector<int> &);
    vector<conditionNode> mConditionNodeVector;
    vector<vector<int> > mContextArray;
    void log(int);
};

#endif
