/*
 *RealDivisionAnalyzer.h
 */

#ifndef REAL_DIVISION_ANALYZER_H
#define REAL_DIVISION_ANALYZER_H

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;

class RealDivisionAnalyzer : public BaseAnalyzer{
public:
	RealDivisionAnalyzer();
	virtual ~RealDivisionAnalyzer();
	virtual void analyzeNode( GNode *, const vector<int> & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
};

#endif /*REAL_DIVISION_ANALYZER_H*/
