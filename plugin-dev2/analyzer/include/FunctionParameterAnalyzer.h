/*
 * FunctionParameterAnalyzer.h
 *
 *  Created on: oct, 10, 2012
 *      Author: kiddlee
 */

#ifndef FUNCTIONPARAMETERANALYZER_H_
#define FUNCTIONPARAMETERANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/FunctionCallNodeProcessor.h"

class FunctionParameterAnalyzer: public BaseAnalyzer {
public:
	FunctionParameterAnalyzer();
	virtual ~FunctionParameterAnalyzer();
	virtual void analyzeNode(GNode *, const vector<int> &);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();

private:
	int threshold;
	int fun_decl_line_num;
	string function_name;
    set<string> params;
};

#endif /* FUNCTIONPARAMETERANALYZER_H_ */
