/*
 * FloatTriCallAnalyzer.h
 *
 *  Created on: 2012-7-30
 *      Author: dandelion
 */

#ifndef FLOATTRICALLANALYZER_H_
#define FLOATTRICALLANALYZER_H_
#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"

class FloatTriCallAnalyzer : public BaseAnalyzer
{
public:
	FloatTriCallAnalyzer();
	virtual ~FloatTriCallAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	bool isConditionFunction(GTree* tree);
};


#endif /* FLOATTRICALLANALYZER_H_ */
