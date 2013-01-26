/*
 *SqrtAnalyzer.h
 */

#ifndef SQRT_ANALYZER_H
#define SQRT_ANALYZER_H

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;

class SqrtAnalyzer : public BaseAnalyzer{
public:
	SqrtAnalyzer();
	virtual ~SqrtAnalyzer();
	virtual void analyzeNode( GNode *, const vector<int> & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
};

#endif /*SQRT_ANALYZER_H*/
