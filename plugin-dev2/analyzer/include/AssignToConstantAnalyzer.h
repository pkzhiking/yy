/*
 * AssignToConstantAnalyzer.h
 *
 *  Created on: Apr 13, 2012
 *      Author: Guo Jiuliang
 */

#ifndef ASSIGN_TO_CONSTANT_ANALYZER_H
#define ASSIGN_TO_CONSTANT_ANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;
class AssignToConstantAnalyzer : public BaseAnalyzer
{
public:
	AssignToConstantAnalyzer();
	virtual ~AssignToConstantAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
};
#endif /* ASSIGN_TO_CONSTANT_ANALYZER_H */
