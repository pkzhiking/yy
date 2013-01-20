/*
 * MacroFunctionAnalyzer.h
 *
 */

#ifndef MACROFUNCTIONANALYZER_H 
#define MACROFUNCTIONANALYZER_H
#include "BaseAnalyzer.h"
#include <vector>
using std::vector;
class MacroFunctionAnalyzer: public BaseAnalyzer
{
public:
	MacroFunctionAnalyzer();
	virtual ~MacroFunctionAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
private:
    int count;
};
#endif /* */
