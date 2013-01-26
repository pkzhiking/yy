/*
 * BadMemOperationCallAnalyzer.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */

#ifndef BAD_MEM_OPERATION_CALL_ANALYZER_H
#define BAD_MEM_OPERATION_CALL_ANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include <vector>
#include "../../core/include/ASTTree.h"
using std::vector;
class BadMemOperationCallAnalyzer : public BaseAnalyzer
{
public:
	BadMemOperationCallAnalyzer();
	virtual ~BadMemOperationCallAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
	void deferedAnalyze(GNode*,const vector<int>&);
private:
	bool isParamPointingToPrimitive(GNode* node);
	bool isParamPointingToPOD(GNode* node);
	vector<GNode*> mNodeArrayToBeAnalyzed;
	vector<vector<int> > mContextArray;
};

#endif /* BAD_MEM_OPERATION_CALL_ANALYZER_H */
