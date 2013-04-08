/*
 * ASTTree.cpp
 *
 *  Created on: Mar 30, 2012
 *      Author: Guo Jiuliang
 */
#include <string>
#include <sstream>
#include <map>
#include "../include/ASTTree.h"
#include <iostream>
using std::ostringstream;
using std::string;
using std::map;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
GTree::GTree()
{
	this->mSize = 0;
}
GNode*
GTree::findNodeByIndex(int index) const
{
    map<int,GNode*>::const_iterator itor = this->mNodeMap.find(index);
    if(itor==this->mNodeMap.end())
    {
        return NULL;
    }
    else
    {
        return itor->second;
    }
}
void
GTree::addTreeNode(GNode* node)
{
    this->mNodeMap[node->getIndex()]=node;
    node->_setTree(this);
}
GTree::~GTree()
{
	for(map<int,GNode*>::iterator itor=mNodeMap.begin();itor!=mNodeMap.end();++itor)
	{
		delete itor->second;
		itor->second = NULL;
	}
	this->mNodeMap.clear();
}

void GTree::Print()
{
    for(map<int, GNode*>::iterator itor = mNodeMap.begin(); itor != mNodeMap.end(); itor++)
    {
	itor->second->Print();
    }
}

string
GTree::toString()
{
	ostringstream sstr;
	for(map<int,GNode*>::iterator itor = this->mNodeMap.begin();itor!=this->mNodeMap.end();++itor)
	{
		sstr<<itor->second->toString()<<"\n";
	}
	return sstr.str();
}
int
GTree:: _getTreeSize() const
{
	return this->mNodeMap.size();
}
void
GTree::_setFullFileName(const char* fullFileName)
{
	this->mFullFileName= fullFileName;
}
void
GTree::_setHashCode(unsigned int hashCode)
{
    this->mHashCode=hashCode;
}
unsigned int
GTree::_getHashCode() const
{
	return this->mHashCode;
}
string
GTree::_getFullFileName() const
{
	return this->mFullFileName;
}
void
GTree::_setFunctionName(const char* functionName)
{
	this->mFunctionName= functionName;
}
string
GTree::_getFunctionName() const
{
	return this->mFunctionName;
}
//GNode implementation
int
GNode::getIndex()
{
	return mIndex;
}
const string&
GNode::getTreeCode()const
{
	return mTreeCode;
}
const GProperty*
GNode::getProperty(const string& propertyName)const
{
	for(vector<pair<string,GProperty> >::const_iterator itor = mPropertyMap.begin();itor!=mPropertyMap.end();++itor)
	{
		if(itor->first == propertyName){
			return &(itor->second);
		}
	}
	return NULL;
    /*
	if(mPropertyMap.find(propertyName)==mPropertyMap.end())
	{
		return NULL;
	}
	else
	{
		return &(mPropertyMap[propertyName]);
	}
	*/
}
void
GNode::_addPropery(const string& propertyName,const GProperty& property)
{
	//mPropertyMap[propertyName] = property;
	mPropertyMap.push_back(make_pair(propertyName,property));
	if (property.ePropertyType == GProperty::PROP_INDEX_TYPE)
	{
		this->mChilds.push_back(property.mNodeProperty);
	}
}
void
GNode::_setIndex(int index)
{
	mIndex = index;
}
void
GNode::_setTreeCode(string treeCode)
{
	mTreeCode = treeCode;
}
void
GNode::_addTreeNode(int index,const string& treeCode)
{
	mIndex = index;
	mTreeCode = treeCode;
}
string
GNode::toString() const
{
    if(NULL == this)
	return "";
	ostringstream sstr;
	sstr<<"@"<<this->mIndex<<"\t"<<this->mTreeCode<<"\t";
//	for(map<string,GProperty>::iterator itor=this->mPropertyMap.begin();itor!=this->mPropertyMap.end();++itor)
//	{
//		sstr<<itor->first<<":"<<itor->second.toString()<<"\t";
//	}
		for(vector<pair<string,GProperty> >::const_iterator itor=this->mPropertyMap.begin();itor!=this->mPropertyMap.end();++itor)
		{
			sstr<<itor->first<<":"<<itor->second.toString()<<"\t";
		}
	return sstr.str();
}
void
GNode::_addChild(int childID)
{
	this->mChilds.push_back(childID);
}

void
GNode::Print()
{
    cout << "@" << mIndex << ":" << endl;
    cout << "	" << mTreeCode << endl;
    for(int i = 0; i < mPropertyMap.size(); i++)
    {
	cout << "   " << mPropertyMap[i].first << ":";
	mPropertyMap[i].second.Print();
    }
    cout << endl;
}

string
GProperty::toString() const
{
    if(NULL == this)
	return "";
	ostringstream sstr;
	if (ePropertyType == GProperty::PROP_STRING_TYPE)
	{
		sstr<<mStringProperty;
	}
	else
	{
		sstr<<"@"<<mNodeProperty;
	}
	return sstr.str();
}

void
GProperty::Print()
{
    if(PROP_STRING_TYPE == ePropertyType)
	cout << mStringProperty;
    else
	cout << mNodeProperty;
}
