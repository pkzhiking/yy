/*
 *ForLoopCombineAnalyzer.h
 */

#ifndef FOR_LOOP_COMBINE_ANALYZER_H
#define FOR_LOOP_COMBINE_ANALYZER_H

#include "BaseAnalyzer.h"
#include <vector>
using std::vector;

class ForLoopCombineAnalyzer : public BaseAnalyzer{
public:
	ForLoopCombineAnalyzer();
	virtual ~ForLoopCombineAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
private:
	bool checkTwoLoop( GNode *, GNode *, const vector< int > & );
	bool checkTwoLoopCond( GNode *, GNode * );
	bool checkTwoLoopExpr( GNode *, GNode * );
	bool checkTwoExpression( GNode *, GNode * );
	bool checkTwoLoopInit( GNode *, GNode * );
};

#endif /*FOR_LOOP_COMBINE_ANALYZER_H*/
