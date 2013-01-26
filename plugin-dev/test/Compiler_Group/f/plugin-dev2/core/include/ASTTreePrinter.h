/*
 * ASTTreePrinter.h
 *
 *  Created on: 2012-7-8
 *      Author: administrator
 */

#ifndef ASTTREEPRINTER_H_
#define ASTTREEPRINTER_H_

#include <queue>
#include <map>
#include "ASTTree.h"
using std::queue;
using std::map;

class ASTTreePrinter
{
public:
	ASTTreePrinter(const GTree* tree);
	~ASTTreePrinter();


private:
			void printASTTree();
   // queue<int> mQueue;
  //  map<int,bool> visitedNode;
     const GTree* mTree;

};


#endif /* ASTTREEPRINTER_H_ */
