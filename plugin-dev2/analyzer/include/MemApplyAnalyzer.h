
#ifndef MEM_APPLY_ANALYZER_H
#define MEM_APPLY_ANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"

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
    int codeLocation;
};

#endif /* MEM_APPLY_ANALYZER_H */
