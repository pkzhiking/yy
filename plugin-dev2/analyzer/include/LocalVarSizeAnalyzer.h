/*
 * LocalVarSizeAnalyzer.h
 *
 *  Created on: Aug 9, 2012
 *      Author: kiddlee
 */

#ifndef LOCALVARSIZEANALYZER_H_
#define LOCALVARSIZEANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"

class LocalVarSizeAnalyzer: public BaseAnalyzer {
public:
	LocalVarSizeAnalyzer();
	virtual ~LocalVarSizeAnalyzer();
	virtual void analyzeNode(GNode *, const vector<int> &);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();

private:
	unsigned long totalLocalVarSize;
	int fun_decl_line_num;
	int threshold;
	string function_name;
};

#endif /* LOCALVARSIZEANALYZER_H_ */
