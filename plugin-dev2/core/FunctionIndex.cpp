/*
 * FunctionIndex.cpp
 *
 *  Created on: 2012-9-10
 *      Author: dandelion
 */


#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <string.h>
#include "include/config.h"
#include "include/FunctionIndex.h"
using std::cout;
using std::endl;
using std::map;
using std::string;
FunctionIndex* FunctionIndex::mFunctionIndex = NULL;

FunctionIndex::FunctionIndex() {

	mFunctionIndexMap.clear();

	char indexLoc[256];
	strcpy(indexLoc, INPUT_FOLD);
	strcat(indexLoc, "index");
 char loc[256], function[256];
	if (freopen(indexLoc, "r", stdin) != NULL){
			while(scanf("%s%s", loc, function) != EOF){
				mFunctionIndexMap[function] = loc;
			}
	}
}

FunctionIndex::~FunctionIndex() {
}

FunctionIndex&
FunctionIndex::getInstance()
{
	if(FunctionIndex::mFunctionIndex==NULL)
	{
		FunctionIndex::mFunctionIndex = new FunctionIndex();
	}
	return *(FunctionIndex::mFunctionIndex);
}

string
FunctionIndex::getLoction(string functionName){
	return mFunctionIndexMap[functionName];
}
