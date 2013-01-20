/*
 *  ApplicationDelegate.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: Guo Jiuliang
 *  Description: Analyzer delegate impl
 */
#include "gcc-plugin.h"
#include "ApplicationDelegate.h"
#include "SrcManager.h"
#include "ASTTree.h"
extern "C" struct ASTTreeBuilder *astTreeBuilder;
extern "C" void gcc_cp_genericize (tree fndecl);
extern GTree *globalASTTreePointer;
ApplicationDelegate::ApplicationDelegate(void* gcc_data,void* user_data):
	mGCCData(gcc_data),
	mUserData(user_data)
{}
void
ApplicationDelegate::run()
{
    globalASTTreePointer = new GTree();
	gcc_cp_genericize((tree)mGCCData);
}
