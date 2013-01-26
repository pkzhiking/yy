/*
 * FunctionCodeSizeAnalyzer.h
 *
 *  Created on: Aug 10, 2012
 *      Author: kiddlee
 */

#ifndef FUNCTIONCODESIZEANALYZER_H_
#define FUNCTIONCODESIZEANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/FunctionCallNodeProcessor.h"

class FunctionCodeSizeAnalyzer: public BaseAnalyzer {
public:
	FunctionCodeSizeAnalyzer();
	virtual ~FunctionCodeSizeAnalyzer();
	virtual void analyzeNode(GNode *, const vector<int> &);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();

private:
	bool printed;
	int codeSize;
	int threshold;
	int fun_decl_line_num;
	string function_name;
};

#endif /* FUNCTIONCODESIZEANALYZER_H_ */
