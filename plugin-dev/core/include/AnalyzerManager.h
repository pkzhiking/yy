/*
 * AnalyzerManager.cpp
 *
 *  Created on: Apr 4, 2012
 *      Author: Guo Jiuliang
 */
#ifndef ANALYZER_MANAGER_H
#define ANALYZER_MANAGER_H
#include <map>
#include <string>
#include <vector>
#include "BaseAnalyzer.h"
using std::map;
using std::string;
using std::vector;
class GNode;
class AnalyzerManager
{
public:
	static AnalyzerManager& getInstance();
	static void destroyInstance();
	void addAnalyzer(BaseAnalyzer*);
	void processNode(GNode*,const vector<int>&);
	void analyzeStart();
	void analyzeEnd();
	void clearAnalyzerState();
private:
	AnalyzerManager();
	AnalyzerManager(AnalyzerManager&){};
	AnalyzerManager& operator=(AnalyzerManager&){return *this;};
	~AnalyzerManager();
private:
	static AnalyzerManager* mAnalyzerManager;
private:
	map<string,vector<BaseAnalyzer*> > mAnalyzersMap;
	vector<BaseAnalyzer*> mAnalyzers;
};
#endif /* ANALYZER_MANAGER_H */
