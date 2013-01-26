/*
 * StructProcessor.cpp
 *
 *  Created on: Apr 30, 2012
 *      Author: Guo Jiuliang
 */
#include "include/StructProcessor.h"
#include "include/NodeProcessor.h"
#include "include/ASTTree.h"
#include "include/Error.h"
#include "../util/include/Util.h"
#include "../util/include/Logger.h"
#include <iomanip>
using std::boolalpha;
using std::setw;
extern GTree *globalASTTreePointer;
StructProcessor* StructProcessor::mStructProcessor = NULL;
StructProcessor&
StructProcessor::getInstance()
{
	if(mStructProcessor==NULL)
	{
		mStructProcessor = new StructProcessor();
	}
	return *mStructProcessor;
}
void
StructProcessor::addStructNode(GNode* node)
{
	if(mStructMap.find(NodeProcessor::getVarTypeName(node))==mStructMap.end())
	{
		StructAdaptor structAdaptor;
		structAdaptor.mName = NodeProcessor::getVarTypeName(node);
		GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
		structAdaptor.mIsPOD = true;
		processStructNode(typeNode,structAdaptor);
		mStructMap[structAdaptor.mName] = structAdaptor;
	}
}
void
StructProcessor::processStructNode(GNode* node,StructAdaptor& structAdaptor) throw (Error)
{
	if(node->getTreeCode()!="record_type")
	{
		throw Error();
	}
	GNode* fieldNode = globalASTTreePointer->findNodeByIndex(node->getProperty("flds")->mNodeProperty);
	while(fieldNode->getTreeCode()!="type_decl")
	{
		if(fieldNode->getTreeCode()=="field_decl")
		{
			processFieldDecl(fieldNode,structAdaptor);
		}
		else if(fieldNode->getTreeCode()=="var_decl")
		{
			processVarDecl(fieldNode,structAdaptor);
		}
		else
		{
			throw Error();
		}
		fieldNode = globalASTTreePointer->findNodeByIndex(fieldNode->getProperty("chan")->mNodeProperty);
	}
}
void
StructProcessor::processFieldDecl(GNode* node,StructAdaptor& structAdaptor)
{
	StructField field;
	field.mFieldName = getFieldName(node);
	field.mFieldSize = getFieldSize(node);
	field.mFieldAlignment = Util::stringToInt(node->getProperty("algn")->mStringProperty);
	field.mIsStatic = false;
	field.mIsPointer = isFieldPointer(node);
	if(isFieldPrimitive(node))
	{
		if(isFieldPointer(node))
		{
			structAdaptor.mIsPOD = false;
		}
	}
	else if(NodeProcessor::isVarRecordType(node))
	{
		//process field member
		addStructNode(node);
		if(!isStructPOD(NodeProcessor::getVarTypeName(node)))
		{
			structAdaptor.mIsPOD = false;
		}
	}
	else
	{
		if(!isStructPOD(NodeProcessor::getVarTypeName(node)))
		{
			structAdaptor.mIsPOD = false;
		}
	}
	structAdaptor.mNonStaticFields.push_back(field);
}
void
StructProcessor::processVarDecl(GNode* node,StructAdaptor& structAdaptor)
{
	StructField field;
	field.mFieldName = getFieldName(node);
	field.mFieldSize = getFieldSize(node);
	field.mFieldAlignment = Util::stringToInt(node->getProperty("algn")->mStringProperty);
	field.mIsStatic = true;
	field.mIsPointer = isFieldPointer(node);
	if(!isFieldPrimitive(node))
	{
		addStructNode(node);
	}
	structAdaptor.mStaticFields.push_back(field);
}
string
StructProcessor::getFieldName(GNode* node)
{
	GNode* idNode = globalASTTreePointer->findNodeByIndex(node->getProperty("name")->mNodeProperty);
	return idNode->getProperty("strg")->mStringProperty;
}
int
StructProcessor::getFieldSize(GNode* node)
{
	GNode* sizeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("size")->mNodeProperty);
	return Util::stringToInt(sizeNode->getProperty("low")->mStringProperty);
}
bool
StructProcessor::isFieldPrimitive(GNode* node)
{
	GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
	return NodeProcessor::isTypePrimitive(typeNode);
}
bool
StructProcessor::isFieldPointer(GNode* node)
{
	GNode* typeNode = globalASTTreePointer->findNodeByIndex(node->getProperty("type")->mNodeProperty);
	if(typeNode->getTreeCode()=="pointer_type")
	{
		return true;
	}
	return false;
}
const vector<StructField>&
StructProcessor::getNonStaticFields(const string& structName)
{
	if(mStructMap.find(structName)==mStructMap.end())
	{
		return mEmptyVector;
	}
	else
	{
		return mStructMap.find(structName)->second.mNonStaticFields;
	}
}
const vector<StructField>&
StructProcessor::getStaticFields(const string& structName)
{
	if(mStructMap.find(structName)==mStructMap.end())
	{
		return mEmptyVector;
	}
	else
	{
		return mStructMap.find(structName)->second.mStaticFields;
	}
}
bool
StructProcessor::isStructPOD(const string& structName)
{
	if(mStructMap.find(structName)!=mStructMap.end())
	{
		return mStructMap.find(structName)->second.mIsPOD;
	}
	//not found
	Logger::i("StructProcessor")<<"struct "<<structName<<" not found."<<endl;
	Logger::i("StructProcessor")<<"mStructMap size="<<mStructMap.size()<<endl;
	return false;
}
void
StructProcessor::printStructResult()
{
	for(map<string,StructAdaptor>::iterator itor=mStructMap.begin();itor!=mStructMap.end();++itor)
	{
		itor->second.printStruct(cout);
	}
}
void
StructAdaptor::printStruct(ostream& out)
{
	out<<setw(14)<<"name:"<<mName<<endl;
	out<<setw(14)<<"isPOD?:"<<boolalpha<<mIsPOD<<endl;
	out<<"*** non-static fields: ***"<<endl;
	out<<"----------------------------"<<endl;
	for(vector<StructField>::iterator itor = mNonStaticFields.begin();itor!=mNonStaticFields.end();++itor)
	{
		out<<setw(14)<<"name:"<<itor->mFieldName<<endl;
		out<<setw(14)<<"size:"<<itor->mFieldSize<<endl;
		out<<setw(14)<<"alignment:"<<itor->mFieldAlignment<<endl;
		out<<setw(14)<<"pointer?:"<<boolalpha<<itor->mIsPointer<<endl;
		out<<setw(14)<<"primitive?:"<<boolalpha<<itor->mIsPrimitive<<endl;
		out<<"----------------------------"<<endl;
	}
	out<<"*** static fields: ***"<<endl;
	out<<"----------------------------"<<endl;
	for(vector<StructField>::iterator itor = mStaticFields.begin();itor!=mStaticFields.end();++itor)
	{
		out<<setw(14)<<"name:"<<itor->mFieldName<<endl;
		out<<setw(14)<<"size:"<<itor->mFieldSize<<endl;
		out<<setw(14)<<"alignment:"<<itor->mFieldAlignment<<endl;
		out<<setw(14)<<"pointer?:"<<boolalpha<<itor->mIsPointer<<endl;
		out<<setw(14)<<"primitive?:"<<boolalpha<<itor->mIsPrimitive<<endl;
		out<<"----------------------------"<<endl;
	}
	out<<"***************************"<<endl;
}

