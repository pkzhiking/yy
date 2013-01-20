/*
 *GlobalVarSizeAnalyzer.h
 */

#ifndef GLOBAL_VAR_SIZE_ANALYZER_H
#define GLOBAL_VAR_SIZE_ANALYZER_H

#include "BaseAnalyzer.h"
#include <vector>
#include <map>
#include <string>
using std::vector;

class GlobalVarSizeAnalyzer : public BaseAnalyzer{
public:
	GlobalVarSizeAnalyzer();
	virtual ~GlobalVarSizeAnalyzer();
	virtual void analyzeNode( GNode *, const vector< int > & );
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
private:
	unsigned long totalGlobalVarSize;
	std::map<string, int> globalVarMap;
};

#endif /*GLOBAL_VAR_SIZE_ANALYZER_H*/
