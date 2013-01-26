/*
 * MultiConditionAnalyzer.h
 *
 *  Created on: Sep 14, 2012
 *      Author: kiddlee
 */

#ifndef MULTICONDITIONANALYZER_H_ 
#define MULTICONDITIONANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"

class MultiConditionAnalyzer: public BaseAnalyzer {
public:
	MultiConditionAnalyzer();
	virtual ~MultiConditionAnalyzer();
	virtual void analyzeNode(GNode *, const vector<int> &);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
    int truth_and_or_if_expr(GNode *);

private:
	int threshold;
};

#endif /* MULTICONDITIONANALYZER_H_ */
