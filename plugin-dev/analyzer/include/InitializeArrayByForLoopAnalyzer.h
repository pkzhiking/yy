/*
 * InitializeArrayByForLoopAnalyzer.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */

#ifndef INITIALIZE_ARRAY_BY_FOR_LOOP_ANALYZER_H
#define INITIALIZE_ARRAY_BY_FOR_LOOP_ANALYZER_H
#include "BaseAnalyzer.h"
#include <vector>
using std::vector;
class InitializeArrayByForLoopAnalyzer : public BaseAnalyzer
{
public:
	InitializeArrayByForLoopAnalyzer();
	virtual ~InitializeArrayByForLoopAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
};
#endif /* INITIALIZE_ARRAY_BY_FOR_LOOP_ANALYZER_H */
