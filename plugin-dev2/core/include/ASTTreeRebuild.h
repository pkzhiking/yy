/*
 * ASTTreeRebuild.h
 *
 *  Created on: 2012-7-7
 *      Author: administrator
 */

#ifndef ASTTREEREBUILD_H_
#define ASTTREEREBUILD_H_
#include "ASTTree.h"

class ASTTreeRebuild
{
	public:
  	ASTTreeRebuild(string);
		ASTTreeRebuild(string fileName, string functionName);
		~ASTTreeRebuild();
		GTree *ASTTreePointer;
	private :
		void init(string fileName);

		string functionName;
};



#endif /* ASTTREEREBUILD_H_ */
