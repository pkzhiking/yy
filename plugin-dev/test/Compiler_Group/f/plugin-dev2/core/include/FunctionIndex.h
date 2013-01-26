/*
 * FunctionIndex.h
 *
 *  Created on: 2012-9-10
 *      Author: dandelion
 */

#ifndef FUNCTIONINDEX_H_
#define FUNCTIONINDEX_H_

#include <string>
#include <utility>
#include <map>
#include <vector>
using std::string;
using std::map;


class FunctionIndex {
public:
	static FunctionIndex& getInstance();
  string getLoction(string);
private:
	FunctionIndex();
	~FunctionIndex();

private:
	map<string, string> mFunctionIndexMap;
	static FunctionIndex *mFunctionIndex ;
};


#endif /* FUNCTIONINDEX_H_ */
