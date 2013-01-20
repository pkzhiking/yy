/*
 *BitNotExprAnalyzer.h
 */

#ifndef BIT_NOT_EXPR_ANALYZER_H
#define BIT_NOT_EXPR_ANALYZER_H

#include "BaseAnalyzer.h"
#include <vector>
using std::vector;

class BitNotExprAnalyzer : public BaseAnalyzer{
public:
	BitNotExprAnalyzer();
	virtual ~BitNotExprAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
};

#endif /*BIT_NOT_EXPR_ANALYZER_H*/
