
#ifndef MEM_APPLY_ANALYZER_H
#define MEM_APPLY_ANALYZER_H
#include "BaseAnalyzer.h"
#include "NodeProcessor.h"
#include "SrcManager.h"
#include "ContextProcessor.h"
#include "Logger.h"

class MemApplyAnalyzer : public BaseAnalyzer
{
public:
	MemApplyAnalyzer();
	virtual ~MemApplyAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
    int memApplyCallCounter;
	string codeLocation;
};

#endif /* MEM_APPLY_ANALYZER_H */
