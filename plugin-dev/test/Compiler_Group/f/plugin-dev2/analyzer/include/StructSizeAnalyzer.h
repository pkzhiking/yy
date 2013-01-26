/*
 * StructSizeAnalyzer.h
 *
 *  Created on: 2012-4-24
 *      Author: dandelion
 */

#ifndef STRUCTSIZEANALYZER_H_
#define STRUCTSIZEANALYZER_H_
#include "../../core/include/BaseAnalyzer.h"
#include "../../core/include/SrcManager.h"
#include "../../core/include/ContextProcessor.h"
#include "../../util/include/Logger.h"
#include <map>
#include <string>
using std::map;
using std::pair;
using std::make_pair;

class StructSizeAnalyzer : public BaseAnalyzer
{
public:
	StructSizeAnalyzer();
	virtual ~StructSizeAnalyzer();
	virtual void analyzeNode(GNode*,const vector<int>&);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
  map <string, bool > structMap;
	bool calStructSize(GNode*);
	int checkUnionType(GNode*);
	GNode* getArrayTypeNode(GNode*);
	string getHashName(GNode*);
	string getStructLocation(GNode*);
};


#endif /* STRUCTSIZEANALYZER_H_ */
