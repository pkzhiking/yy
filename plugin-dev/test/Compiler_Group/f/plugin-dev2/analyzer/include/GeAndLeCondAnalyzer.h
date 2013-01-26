/*
 * GeAndLeCondAnalyzer.h
 *
 */

#ifndef GEANDLECONDANALYZER_H 
#define GEANDLECONDANALYZER_H
#include "../../core/include/BaseAnalyzer.h"
#include <vector>
using std::vector;
class GeAndLeCondAnalyzer: public BaseAnalyzer
{
public:
	GeAndLeCondAnalyzer();
	virtual ~GeAndLeCondAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState(){};
private:
};
#endif /* */
