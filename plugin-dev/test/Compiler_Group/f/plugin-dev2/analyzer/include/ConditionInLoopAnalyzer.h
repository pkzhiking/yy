/*
 * ConditionInLoopAnalyzer.h
 *
 *  Created on: 2012-10-16
 *      Author: dandelion
 */

#ifndef ConditionInLoopAnalyzer_H_
#define ConditionInLoopAnalyzer_H_

#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/ASTTree.h"
using std::vector;
class ConditionInLoopAnalyzer : public BaseAnalyzer
{
public:

	ConditionInLoopAnalyzer();
	virtual ~ConditionInLoopAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};

private:

};


#endif /* ConditionInLoopAnalyzer_H_ */
