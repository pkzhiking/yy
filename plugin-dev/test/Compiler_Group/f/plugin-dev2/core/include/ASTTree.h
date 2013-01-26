/*
 * ASTTree.h
 *
 *  Created on: Mar 30, 2012
 *      Author: Guo Jiuliang
 */
#ifndef ASTTREE_H
#define ASTTREE_H

#include <map>
#include <string>
#include <vector>
#include <utility>
using std::map;
using std::string;
using std::vector;
using std::pair;
/* use GCC internal tree code name array */
extern const char *const tree_code_name[];
class GNode;
class GProperty;
///generic tree wrapper class                                                  
class GTree 
{
    public:
        GTree();
        GNode* findNodeByIndex(int nodeIndex) const;
        void addTreeNode(GNode* node);
        string toString();
        int _getTreeSize() const;
        void _setFullFileName(const char*);
        string _getFullFileName() const;
        void _setFunctionName(const char*);
        string _getFunctionName() const;
        void _setHashCode(unsigned int);
        unsigned int _getHashCode() const;
        ~GTree();
    private:
        //member functions to delete
        GTree(const GTree&){}
        GTree& operator=(const GTree&){return *this;}
    private:
        map<int,GNode*> mNodeMap;
        string mFullFileName, mFunctionName;
        int mSize;
        unsigned int mHashCode;

};
///generic tree node wrapper class
class GNode
{
    friend class GTree;
    public:
        int getIndex();
        const string& getTreeCode()const;
        const GProperty* getProperty(const string& propertyName)const;
        const vector<pair<string,GProperty> >& getProperty(){return mPropertyMap;}
        void _addPropery(const string& propertyName,const GProperty& property);
        void _setIndex(int index);
        void _setTreeCode(string treeCode);
        void _addTreeNode(int index,const string& treeCode);
        void _setTree(GTree* tree){this->mTree = tree;}
        void _addChild(int childID);
        GTree* _getTree(){return this->mTree;}
        const vector<int>& getChilds(){return mChilds;}
        string toString() const;
    private:
    int mIndex;
    string mTreeCode;
    //map<string,GProperty> mPropertyMap;
    vector<pair<string,GProperty> > mPropertyMap;
    vector<int> mChilds;
    GTree *mTree;
};
///property struct
class GProperty
{
    public:
        enum ValueTypeEnum {PROP_STRING_TYPE,PROP_INDEX_TYPE};
        ValueTypeEnum ePropertyType;
        string mStringProperty;
        int mNodeProperty;
    public:
        string toString() const;
};
#endif /* ASTTREE_H */
