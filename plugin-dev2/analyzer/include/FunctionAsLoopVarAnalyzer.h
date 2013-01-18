/*
 * FunctionAsLoopVarAnalyzer.h
 *
 *  Created on: 2012-8-10
 *      Author: dandelion
 */

#ifndef FUNCTIONASLOOPVARANALYZER_H_
#define FUNCTIONASLOOPVARANALYZER_H_


#include "../../core/include/BaseAnalyzer.h"
#include "../../util/include/Logger.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
class FunctionAsLoopVarAnalyzer : public BaseAnalyzer{
public:
	FunctionAsLoopVarAnalyzer();
	virtual ~FunctionAsLoopVarAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:

};


#endif /* FUNCTIONASLOOPVARANALYZER_H_ */
