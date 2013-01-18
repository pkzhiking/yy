/*
 * MultiplyByTwosExponentAnalyzer.h
 *
 *  Created on: Apr 24, 2012
 *      Author: Guo Jiuliang
 */

#ifndef MULTIPLYBYTWOSEXPONENTANALYZER_H
#define MULTIPLYBYTWOSEXPONENTANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;
class MultiplyByTwosExponentAnalyzer: public BaseAnalyzer
{
public:
	MultiplyByTwosExponentAnalyzer();
	virtual ~MultiplyByTwosExponentAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	bool ifTwosExponent(int);
};
#endif /* MULTIPLYBYTWOSEXPONENTANALYZER_H */
