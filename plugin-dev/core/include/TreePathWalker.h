/*
 * TreePathWalker.cpp
 *
 *  Created on: Mar 30, 2012
 *      Author: Guo Jiuliang
 */
#ifndef TREE_PATH_WALKER_H
#define TREE_PATH_WALKER_H
#include <vector>
#include <map>
#include "BaseAnalyzer.h"
#include "Logger.h"
using std::vector;
using std::map;
class GTree;
class TreePathWalker
{
public:
	TreePathWalker(const GTree* tree);
	~TreePathWalker();
    void startTreeWalk();
    void finishTreeWalk();
    void processNode(int nodeIndex);
    void walkOneStep();
    void updateWalkerPathContext(int);
public:
    static void addAnalyzer(BaseAnalyzer*);
private:
    vector<int> mWalkerQueue;
    vector<int> mWalkerPathContext;
    map<int,int> mExecuteTracePosMap;
    const GTree* mTree;

};
#endif /* TREE_PATH_WALKER_H */
