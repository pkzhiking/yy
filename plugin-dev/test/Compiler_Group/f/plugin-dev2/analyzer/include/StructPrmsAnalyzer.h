/*
 * StructPrmsAnalyzer.h
 *
 *  Created on: 2012-11-6
 *      Author: dandelion
 */

#ifndef STRUCTPRMSANALYZER_H_
#define STRUCTPRMSANALYZER_H_

#include "../../core/include/BaseAnalyzer.h"
#include "../../util/include/Logger.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
class StructPrmsAnalyzer : public BaseAnalyzer{
public:
	StructPrmsAnalyzer();
	virtual ~StructPrmsAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	 bool judged;
};



#endif /* STRUCTPRMSANALYZER_H_ */
