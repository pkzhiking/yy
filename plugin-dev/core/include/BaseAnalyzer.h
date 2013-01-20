/*
 * BaseAnalyzer.h
 *
 *  Created on: Apr 3, 2012
 *      Author: Guo Jiuliang
 */
#ifndef BASE_ANALYZER_H
#define BASE_ANALYZER_H
#include <string>
#include <vector>
using std::string;
using std::vector;
class GNode;
class BaseAnalyzer
{
public:
	BaseAnalyzer(){};
	virtual ~BaseAnalyzer()=0;
	//this callback function is called before tree walker
	//really start to make tree walk,often you should make
	//some initialization task here
	virtual void startAnalyze()=0;
	//this callback function is called each time tree walker
	//walk through the node with type of this analyzer registered
	virtual void analyzeNode(GNode*,const vector<int>&)=0;
	//this callback function is called to notify the finish of
	//analyzer call,often you should make report here
	virtual void finishAnalyze()=0;
	//this callback function should be used to restore analyzer
	//to initial state for analyze another new function
	virtual void clearAnalyzerState()=0;
	const string& getAnalyzerNodeType() const
	{return mAnalyzerNodeType;}
	void setAnalyzerNodeType(const string&);
private:
	string mAnalyzerNodeType;
};
#endif /* BASE_ANALYZER_H */



