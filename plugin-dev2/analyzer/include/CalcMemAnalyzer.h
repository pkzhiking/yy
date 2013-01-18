#ifndef CALCMEM_H
#define CALCMEM_H

#include "../../core/include/BaseAnalyzer.h"
#include <vector>

using std::vector;

class CalcMemAnalyzer : public BaseAnalyzer
{
public:
    CalcMemAnalyzer();
    virtual ~CalcMemAnalyzer();
    virtual void analyzeNode(GNode *, const vector<int> &);
    virtual void startAnalyze();
    virtual void finishAnalyze();
    virtual void clearAnalyzerState();
};

#endif
