/*
 * ASTTreeBuilder.cpp
 *
 *  Created on : Apr 2, 2012
 *      Author : Guo Jiuliang
 *  Description: ASTTreeBuilder implementation
 */
#include "ASTTreeBuilder.h"
#include "ASTTree.h"
#include "Logger.h"
#include "ASTTreePrinter.h"
#include <iostream>
#include <sstream>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
struct ASTTreeBuilder astTreeBuilderInstance;
struct ASTTreeBuilder *astTreeBuilder = &astTreeBuilderInstance;
GTree *globalASTTreePointer = NULL;
GNode *temporaryNode = NULL;
extern "C" void walker_add_index(int);
extern "C" void walker_add_property(const char*,const char*);
extern "C" void walker_add_child(const char*,int);
extern "C" void walker_add_node(int,const char*);
extern "C" void walker_add_ulong_property(const char*,unsigned long);
extern "C" void walker_add_int_perperty(const char*,int);
extern "C" void walker_add_real_property(const char* name,const char*);
extern "C" void walker_add_fixed_property(const char*,const char*);
extern "C" void walker_add_string_property(const char*,const char*);
extern "C" void walker_add_string(const char*);
extern "C" void walker_init();
extern "C" void walker_finish_function();

extern "C" void walker_add_function_name(const char*);

//implementation
extern "C" void walker_add_function_name(const char* functionName)
{
	globalASTTreePointer->_setFunctionName(functionName);
}

extern "C" void walker_add_index(int index)
{
	//cout<<"walker_add_index"<<endl;
}
void walker_add_property(const char* name,const char* property)
{
	//cout<<"walker_add_property"<<endl;
}
void walker_add_child(const char* name,int index)
{
	Logger::d("walker_add_child name:")<<name<<" index:"<<index<<endl;
	//cout<<"walker_add_child name:"<<name<<" index:"<<index<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_INDEX_TYPE;
	prop.mNodeProperty = index;
	temporaryNode->_addPropery(name,prop);
}
void walker_add_node(int index,const char* treeCode)
{
	temporaryNode = new GNode();
	temporaryNode->_addTreeNode(index,treeCode);
	globalASTTreePointer->addTreeNode(temporaryNode);
	Logger::d("add node with index ")<<index<<" code:"<<treeCode<<endl;
	//cout<<"add node with index "<<index<<" code:"<<treeCode<<endl;
}
void walker_add_ulong_property(const char* name,unsigned long property)
{
	Logger::d("add unsigned long property name:")<<name<<" value:"<<property<<endl;
	//cout<<"add unsigned long property name:"<<name<<" value:"<<property<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_STRING_TYPE;
	ostringstream sstr;
	sstr<<property;
	prop.mStringProperty = sstr.str();
	temporaryNode->_addPropery(name,prop);
}
void walker_add_int_property(const char* name,int property)
{
	Logger::d("add int property name:")<<name<<" value:"<<property<<endl;
	//cout<<"add int property name:"<<name<<" value:"<<property<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_STRING_TYPE;
	ostringstream sstr;
	sstr<<property;
	prop.mStringProperty = sstr.str();
	temporaryNode->_addPropery(name,prop);
}
void walker_add_real_property(const char* name,const char* property)
{
	//cout<<"add real property name:"<<name<<" value:"<<property<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_STRING_TYPE;
	ostringstream sstr;
	sstr<<property;
	prop.mStringProperty = sstr.str();
	temporaryNode->_addPropery(name,prop);
}
void walker_add_fixed_property(const char* name,const char* property)
{
	Logger::d("add fixed property name:")<<name<<" value:"<<property<<endl;
	//cout<<"add fixed property name:"<<name<<" value:"<<property<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_STRING_TYPE;
	ostringstream sstr;
	sstr<<property;
	prop.mStringProperty = sstr.str();
	temporaryNode->_addPropery(name,prop);
}
void walker_add_string_property(const char* name,const char* property)
{
	Logger::d("add string property name:")<<name<<" value:"<<property<<endl;
	//cout<<"add string property name:"<<name<<" value:"<<property<<endl;
	GProperty prop;
	prop.ePropertyType = GProperty::PROP_STRING_TYPE;
	ostringstream sstr;
	sstr<<property;
	prop.mStringProperty = sstr.str();
	temporaryNode->_addPropery(name,prop);
}
void walker_add_string(const char* input)
{
	//cout<<"add string:"<<input<<endl;
}
void walker_finish_function()
{
	//start analyze here
	//after analyzing, clear memory
	//cout<<globalASTTreePointer->toString()<<endl;

	ASTTreePrinter treePrinter(globalASTTreePointer);
	delete globalASTTreePointer;
	globalASTTreePointer = NULL;
}
//when initializing walker, add analyzers here
extern "C" void walker_init()
{
	Logger::d("walker_init")<<"init walker"<<endl;
	astTreeBuilder->addChild = walker_add_child;
	astTreeBuilder->addIndex = walker_add_index;
	astTreeBuilder->addProperty = walker_add_property;
	astTreeBuilder->addNode = walker_add_node;
	astTreeBuilder->addULongProperty = walker_add_ulong_property;
	astTreeBuilder->addIntProperty = walker_add_int_property;
	astTreeBuilder->addRealProperty = walker_add_real_property;
	astTreeBuilder->addFixedProperty = walker_add_fixed_property;
	astTreeBuilder->addStringProperty = walker_add_string_property;
	astTreeBuilder->addString = walker_add_string;
	astTreeBuilder->finishFunction = walker_finish_function;
	astTreeBuilder->addFunctionName = walker_add_function_name;
	globalASTTreePointer = new GTree();
}
