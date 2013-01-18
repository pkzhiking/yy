/*
 * BaseAnalyzer.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include "../include/BaseAnalyzer.h"
BaseAnalyzer::~BaseAnalyzer() {
}
void BaseAnalyzer::setAnalyzerNodeType(const string& nodeType) {
	mAnalyzerNodeType.insert(nodeType);
}
//const string&
//BaseAnalyzer::getAnalyzerNodeType() const
//{
//	return mAnalyzerNodeType;
//}
