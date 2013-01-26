/*
 * DivideByTwosExponentAnalyzer.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */

#ifndef DIVIDE_BY_TWOS_EXPONENT_ANALYZER_H
#define DIVIDE_BY_TWOS_EXPONENT_ANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;
class DivideByTwosExponentAnalyzer : public BaseAnalyzer
{
public:
	DivideByTwosExponentAnalyzer();
	virtual ~DivideByTwosExponentAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	bool ifTwosExponent(int);
};
#endif /* DIVIDE_BY_TWOS_EXPONENT_ANALYZER_H */
