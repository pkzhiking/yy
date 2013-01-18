/*
 * IfToSwitchAnalyzer.h
 *
 *  Created on: 2012-8-7
 *      Author: dandelion
 */

#ifndef IFTOSWITCHANALYZER_H_
#define IFTOSWITCHANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include <vector>
#include "../../core/include/ASTTree.h"
using std::vector;
class IfToSwitchAnalyzer : public BaseAnalyzer
{
public:
	struct conditionNode{
		int varId;
		int iContextLine;
		int father;
		GNode* node;
	};
	IfToSwitchAnalyzer();
	virtual ~IfToSwitchAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
	static bool getVarIDOrder(conditionNode a, conditionNode b);
private:
	void isIfToSwitch(const vector<int>&);
	vector< conditionNode > mConditionNodeVector;
	vector<vector<int> > mContextArray;
};


#endif /* IFTOSWITCHANALYZER_H_ */
