/*
 * ASTTreePrinter.cpp
 *
 *  Created on: 2012-7-8
 *      Author: administrator
 */


#include "ASTTreePrinter.h"
#include <queue>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include "SrcManager.h"
#include <iostream>
#include <string.h>
#include "ASTTree.h"
#include "Config.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
using std::queue;
using std::ostream_iterator;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
ASTTreePrinter::ASTTreePrinter(const GTree* tree):
mTree(tree)
{
	printASTTree();
}
ASTTreePrinter::~ASTTreePrinter()
{
}
void ASTTreePrinter::printASTTree()
{
	FILE *stream;
	char name[256];

	string fileName = SrcManager::getInstance().getFileName();
	string dir = SrcManager::getInstance().getFilePath();
	string sDir = SOURCEFILE_LOCATION;
  int loc;

	if(dir.find(sDir) == string::npos){
		return ;
	}

	if (sDir[sDir.size()-1] != '/'){
		sDir += "/";
	}
	if (dir[dir.size()-1] != '/'){
			dir += "/";
	}
//创建目录
	int st, sl;
	st = sl = sDir.length();
	while( (loc =dir.find("/", st)) != string::npos){
		string nDir = dir.substr(sl, loc - sl + 1);
		st = loc + 1;
		nDir = INPUT_FOLD + nDir;
		if (opendir(nDir.c_str()) == NULL){
			mkdir(nDir.c_str(),0700);

		}
	}

	fileName = fileName.substr(0, fileName.find("."));
	strcpy(name, INPUT_FOLD);
	strcat(name, dir.substr(sDir.length()).c_str());
	strcat(name,fileName.c_str());

	strcat(name, "$&$");
	string functionName = this->mTree->_getFunctionName();
	functionName = functionName.substr(functionName.find(" ") + 1);
	functionName = functionName.substr(0, functionName.find("("));
	strcat(name, functionName.c_str());
	strcat(name,".printer");
	stream = fopen (name, "w");
	if (!stream)
	{
		//TODO: REPORT ERROR
		return ;
	}

	//record index
	FILE *index;
	char indexLoc[256];
	strcpy(indexLoc, INPUT_FOLD);
	strcat(indexLoc, "index");
	index = fopen(indexLoc, "a");
	string indexLine(name);
	indexLine += " " + fileName + "$&$" + functionName;
//	string bb = cc;
//	int pos;
//	if(   (pos=cc.find("/home/dandelion/new/plugin-dev2/"))!=string::npos   )
//	            bb.replace(pos,32,"/home/dandelion/");

	fprintf(index, "%s\n" ,indexLine.c_str());
	fclose(index);

	unsigned int hash = 0; // tree hash code
	fprintf(stream, "%s\n",  SrcManager::getInstance().getFullFileName().c_str());


	int totalNode = this->mTree->_getTreeSize();

	for(int nodeID = 1; nodeID <= totalNode; nodeID++)
	{
		GNode* currentNode = this->mTree->findNodeByIndex(nodeID);
		fprintf(stream, "%d %s\n", currentNode->getIndex(), currentNode->getTreeCode().c_str());
		hash = ASTTreePrinter::BKDRHash(currentNode->getTreeCode(), hash);
		const vector< pair<string,GProperty> >& propertyVector =  currentNode->getProperty();
		fprintf(stream, "mPropertyMap %d\n", propertyVector.size());
		hash = ASTTreePrinter::BKDRHash(propertyVector.size(), hash);
		for(vector<pair<string,GProperty> >::const_iterator itor = propertyVector.begin();itor!=propertyVector.end();++itor)
		{
			const GProperty&  property = itor->second;
			 if(property.ePropertyType == GProperty::PROP_INDEX_TYPE)
			 {
					fprintf(stream, "1\n%s\n%d\n", itor->first.c_str(), property.mNodeProperty);

			 }else
			 {
					fprintf(stream, "0\n%s\n%s\n", itor->first.c_str(), property.mStringProperty.c_str());
			 }
		}
	}
	hash = (hash & 0x7FFFFFFF);
	fprintf(stream, "%d\n", hash);
	fclose(stream);


}
unsigned int ASTTreePrinter::BKDRHash(string str, unsigned int hash)
{
    unsigned int seed = 131313; // 31 131 1313 13131 131313 etc..
    int i = 0;
    while (i < str.length())
    {
        hash = hash * seed + (str[i++]);
    }

    return hash;
}
unsigned int ASTTreePrinter::BKDRHash(int var, unsigned int hash)
{
    unsigned int seed = 131313; // 31 131 1313 13131 131313 etc..

    hash = hash * seed + var;

    return hash;
}
