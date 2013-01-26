/*
 * FunctionCallAnalyzer.h
 *
 *  Created on: 2012-7-30
 *      Author: dandelion
 */

#ifndef FUNCTIONCALLANALYZER_H_
#define FUNCTIONCALLANALYZER_H_
#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"

class FunctionCallAnalyzer : public BaseAnalyzer
{
public:
	FunctionCallAnalyzer();
	virtual ~FunctionCallAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	bool isConditionFunction(GTree* tree);
};


#endif /* FUNCTIONCALLANALYZER_H_ */
