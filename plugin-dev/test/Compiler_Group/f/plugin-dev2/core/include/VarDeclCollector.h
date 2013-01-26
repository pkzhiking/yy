/*
 * VarDeclCollector.h
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#ifndef VAR_DECL_COLLECTOR_H
#define VAR_DECL_COLLECTOR_H
#include "BaseAnalyzer.h"
#include "ASTTree.h"
#include <vector>
using std::vector;
class VarDeclCollector : public BaseAnalyzer
{
public:
	VarDeclCollector();
	virtual ~VarDeclCollector();
	virtual void analyzeNode(GNode*,const vector<int>& context);
	virtual void startAnalyze();
	virtual void finishAnalyze();
	virtual void clearAnalyzerState();
private:
	vector<GNode*> mVarNameList;
	vector<int> mSrcPositionList;
	//string mAnalyzerNodeType;
private:
	bool isVarPrimitive(GNode* node);
};
#endif /* VAR_DECL_COLLECTOR_H */



