/*
 * FindSameFunctionAnalyzer.h
 *
 *  Created on: 2012-10-23
 *      Author: dandelion
 */

#ifndef FINDSAMEFUNCTIONANALYZER_H_
#define FINDSAMEFUNCTIONANALYZER_H_

#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"

class FindSameFunctionAnalyzer
{
public:
	static FindSameFunctionAnalyzer& getInstance();
  void globalAnalyze(unsigned int hashCode, string function);
private:
	 map <unsigned int , string > mfunctionHashMap;
public:
	 FindSameFunctionAnalyzer();
	 ~FindSameFunctionAnalyzer();
private:
	 static FindSameFunctionAnalyzer *mFindSameFunctionAnalyzer;
};



#endif /* FINDSAMEFUNCTIONANALYZER_H_ */
