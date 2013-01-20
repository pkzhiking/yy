/*
 *ForLoopToZeroAnalyzer.h
 */

#ifndef FOR_LOOP_TO_ZERO_ANALYZER_H
#define FOR_LOOP_TO_ZERO_ANALYZER_H

#include "BaseAnalyzer.h"
#include <vector>
#include <string>
using std::vector;

class ForLoopToZeroAnalyzer : public BaseAnalyzer{
public:
	ForLoopToZeroAnalyzer();
	virtual ~ForLoopToZeroAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
private:
	GNode *stmtNode;
	vector< string > condVarVec, iterVarVec;
	void findVarInCond( GNode * ); 
	void findIterWithExpr( GNode * );
	void checkInitStmt( GNode *, const vector< int > & );
	bool isStrInVector( string &, vector< string > & );
};

#endif /*FOR_LOOP_TO_ZERO_ANALYZER_H*/
