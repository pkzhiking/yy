#ifndef DIVIDEBYCONSTANT_H
#define DIVIDEBYCONSTANT_H

#include "BaseAnalyzer.h"
#include <vector>

using std::vector;

class DivideByConstantAnalyzer : public BaseAnalyzer
{
public:
    DivideByConstantAnalyzer();
    virtual ~DivideByConstantAnalyzer();
    virtual void analyzeNode(GNode*, const vector<int>&);
    virtual void startAnalyze();
    virtual void finishAnalyze();
    virtual void clearAnalyzerState(){};
};


#endif
