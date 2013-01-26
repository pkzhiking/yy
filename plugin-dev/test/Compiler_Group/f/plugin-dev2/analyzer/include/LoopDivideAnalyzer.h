/*
 * LoopDivideAnalyzer.h
 *
 *  Created on: Aug 9, 2012
 *      Author: kiddlee
 */

#ifndef LOOPDIVIDEANALYZER_H_
#define LOOPDIVIDEANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;

class LoopDivideAnalyzer: public BaseAnalyzer {
public:
	LoopDivideAnalyzer();
	virtual ~LoopDivideAnalyzer();
	virtual void analyzeNode(GNode *, const vector<int> &);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();

private:
	int CountCodesInLoop(GNode * node);
	int CountCodesInBindExpr(GNode * node);
	int CountCodesInCondExpr(GNode * node);
	int CountCodesInCleanUpPointExpr(GNode * node);
	int CountCodesInFunCall(GNode *node);
	int threshold;
	int loopFactor;
};

#endif /* LOOPDIVIDEANALYZER_H_ */
