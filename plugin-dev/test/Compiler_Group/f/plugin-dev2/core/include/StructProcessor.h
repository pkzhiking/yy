/*
 * StructProcessor.h
 *
 *  Created on: Apr 30, 2012
 *      Author: Guo Jiuliang
 */

#ifndef STRUCT_PROCESSOR_H
#define STRUCT_PROCESSOR_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "ASTTree.h"
#include "Error.h"
using std::vector;
using std::map;
using std::string;
using std::ostream;
using std::cout;
struct StructField
{
	string mFieldName;
	int mFieldSize;
	int mFieldAlignment;
	bool mIsStatic;
	bool mIsPrimitive;
	bool mIsPointer;
};
struct StructAdaptor
{
	string mName;
	vector<StructField> mNonStaticFields;
	vector<StructField> mStaticFields;
	bool mIsPOD;
	void printStruct(ostream& out);
};
class StructProcessor
{
public:
	static StructProcessor& getInstance();
	//var_decl->record_type
	void addStructNode(GNode* node);
	const vector<StructField>& getNonStaticFields(const string& structName);
	const vector<StructField>& getStaticFields(const string& structName);
	bool isStructPOD(const string& structName);
	void printStructResult();
private:
	void processStructNode(GNode* node,StructAdaptor& structAdaptor) throw (Error);
	void processFieldDecl(GNode* node,StructAdaptor& structAdaptor);
	void processVarDecl(GNode* node,StructAdaptor& structAdaptor);
	string getFieldName(GNode* node);
	int getFieldSize(GNode* node);
	bool isFieldPrimitive(GNode* node);
	bool isFieldPointer(GNode* node);
private:
	static StructProcessor* mStructProcessor ;
	map<string,StructAdaptor> mStructMap;
	vector<StructField> mEmptyVector;
};
#endif /* STRUCT_PROCESSOR_H */
