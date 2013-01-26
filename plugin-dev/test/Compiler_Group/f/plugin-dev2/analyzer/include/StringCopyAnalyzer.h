/*
 * StringCopyAnalyzer.h
 *
 *  Created on: 2012-8-5
 *      Author: dandelion
 */

#ifndef STRINGCOPYANALYZER_H_
#define STRINGCOPYANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"

class StringCopyAnalyzer : public BaseAnalyzer
{
public:
	StringCopyAnalyzer();
	virtual ~StringCopyAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
};


#endif /* STRINGCOPYANALYZER_H_ */
