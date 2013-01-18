#ifndef REDUCIBLE_H
#define REDUCIBLE_H

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
#include <string>
#include "../../core/include/ASTTree.h"

using std::vector;
using std::string;

struct LogicUnit
{
public:
    bool anti(const LogicUnit &);
    LogicUnit(){ope = left = right = "";}
    string ope;
    string left;
    string right;
};

class ReducibleIfAnalyzer : public BaseAnalyzer
{
public:
    ReducibleIfAnalyzer();
    virtual ~ReducibleIfAnalyzer();
    virtual void startAnalyze();
    virtual void finishAnalyze();
    virtual void analyzeNode(GNode *, const vector<int> &);
    virtual void clearAnalyzerState();
private:
    vector<LogicUnit> vec_LogicUnit;
};

#endif
