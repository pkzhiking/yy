/*
 * StructSizeAnalyzer.cpp
 *
 *  Created on: 2012-4-24
 *      Author: dandelion
 */
#include "include/StructSizeAnalyzer.h"
#include "../core/include/ASTTree.h"
#include "../core/include/NodeProcessor.h"
#include "../core/include/ReportManager.h"
#include "../core/include/TotalProblem.h"
#include <sstream>
using std::stringstream;
extern GTree *globalASTTreePointer;
StructSizeAnalyzer::StructSizeAnalyzer() {
	setAnalyzerNodeType("record_type");
}
StructSizeAnalyzer::~StructSizeAnalyzer() {

}
void StructSizeAnalyzer::clearAnalyzerState() {

}
string StructSizeAnalyzer::getHashName(GNode* node){


	if (node->getProperty("name") == NULL)
		    return "";
	GNode*	nameNode = globalASTTreePointer->findNodeByIndex(node->getProperty(
				"name")->mNodeProperty);

	if (nameNode->getProperty("srcp") == NULL)
		return "";
	string hashStr =  nameNode->getProperty("srcp")->mStringProperty;
	hashStr = hashStr.substr(0, hashStr.find(":"));
	hashStr += globalASTTreePointer->findNodeByIndex(nameNode->getProperty("name")->mNodeProperty)->getProperty("strg")->mStringProperty;
	return hashStr;
}
void StructSizeAnalyzer::analyzeNode(GNode* node, const vector<int>& context) {
	string nodeStr = getHashName(node);
	if (structMap.find(nodeStr) != structMap.end()) {
		return;
	}

	if (!calStructSize(node)) {

		stringstream sstr;
		sstr<<getStructLocation(node);
		int lineNumber;
		sstr>>lineNumber;



		structMap[nodeStr] = false;
//		Logger::a("StructSizeAnalyzer") << "struct is not 4bytes! "
//				<< globalASTTreePointer->findNodeByIndex(
//						globalASTTreePointer->findNodeByIndex(
//								node->getProperty("name")->mNodeProperty) ->getProperty(
//								"name")->mNodeProperty)->getProperty("strg")->mStringProperty
//				<< " " << SrcManager::getInstance().getFullFileName()\
//				<< ":" << lineNumber << endl;

	//	getStructLocation(node);
		stringstream reportMsgStream;
						reportMsgStream<<"StructSizeAnalyzer: struct size is not 32-bit aligned! " << endl;
						string reportMsg = reportMsgStream.str();
						ReportManager::getInstance().insertReport(SrcManager::getInstance().getFullFileName(),lineNumber,reportMsg);
						ProblemList::GetInstance().Add("StructSizeAnalyzer", SrcManager::getInstance().getFullFileName(), lineNumber);
	}

}
string StructSizeAnalyzer::getStructLocation(GNode* node){
	string location =  globalASTTreePointer->findNodeByIndex(node->getProperty(
			"name")->mNodeProperty)->getProperty("srcp")->mStringProperty;
	location = location.substr(location.find(":") + 1, location.length() -  location.find(":") - 1);
	return location;
}
void StructSizeAnalyzer::startAnalyze() {

}
void StructSizeAnalyzer::finishAnalyze() {

}
GNode* StructSizeAnalyzer::getArrayTypeNode(GNode* node) {
	GNode* typeNode = node;
	while (!typeNode->getProperty("elts"))
	{
		typeNode = globalASTTreePointer->findNodeByIndex(typeNode->getProperty("elts")->mNodeProperty);
	}
	return typeNode;
}

int StructSizeAnalyzer::checkUnionType(GNode* node) {

	if (node->getProperty("flds") == NULL)
		return 0;
	GNode* fieldNode = globalASTTreePointer->findNodeByIndex(node->getProperty(
			"flds")->mNodeProperty);

	int size = 0, mSize = -1;
	while (true) {
		if (!fieldNode->getProperty("chan")) {
			break;
		}
		if (fieldNode->getTreeCode() == "field_decl") {
			GNode* typeNode = globalASTTreePointer->findNodeByIndex(
					fieldNode->getProperty("type")->mNodeProperty);
			GNode* sizeNode = globalASTTreePointer->findNodeByIndex(
					fieldNode->getProperty("size")->mNodeProperty);
			int tSize = atoi(
					sizeNode->getProperty("low")->mStringProperty.c_str());
			if (tSize > mSize) {
				mSize = tSize;
				switch (NodeProcessor::getNodeType(typeNode)) {
				case 1: {//基本数据类型
					mSize = size = tSize;
					break;
				}
				case 2: {//数组
					GNode* arrayType = getArrayTypeNode(typeNode);
					if (NodeProcessor::getNodeType(arrayType) == 3)//struct array
					{
						if (calStructSize(typeNode)) {
							size = 0;
						} else {
							size = -1;
						}
					} else {
						int
								arraySize =
										atoi(
												globalASTTreePointer->findNodeByIndex(
														typeNode->getProperty(
																"size")->mNodeProperty)->getProperty(
														"low")->mStringProperty.c_str());

						if (!arraySize)//大小未确定多维数组不在结构体内计算
							break;

						mSize = arraySize;
						size = mSize % 32;

					}
					break;
				}
				case 3: {//结构体

					if (calStructSize(typeNode)) {
						size = 0;
					} else {
						size = -1;
					}
					break;
				}
				case 4: {//union
					size = checkUnionType(typeNode);
					break;
				}
				default: {
					size = -1;
					break;
				}
				}//switch
			}//if >
		}//if
		fieldNode = globalASTTreePointer->findNodeByIndex(
				fieldNode->getProperty("chan")->mNodeProperty);
	}//while

	return size;
}
bool StructSizeAnalyzer::calStructSize(GNode* node) {
	string nodeStr = getHashName(node);

	if (structMap.find(nodeStr) != structMap.end()) {
		return structMap[nodeStr];
	}

    const GProperty * fldsNode = node->getProperty("flds");
    GNode* fieldNode = NULL;
    if(fldsNode!=0){
        fieldNode = globalASTTreePointer->findNodeByIndex(fldsNode->mNodeProperty);
    }else{
        return false;
    }
    /*
	GNode* fieldNode = globalASTTreePointer->findNodeByIndex(node->getProperty(
			"flds")->mNodeProperty);
    */

	int size = 0;
	while (true) {
		if (!fieldNode->getProperty("chan")) {
			break;
		}
		if (fieldNode->getTreeCode() == "field_decl") {
			GNode* typeNode = globalASTTreePointer->findNodeByIndex(
					fieldNode->getProperty("type")->mNodeProperty);
			GNode* sizeNode = globalASTTreePointer->findNodeByIndex(
					fieldNode->getProperty("size")->mNodeProperty);
			switch (NodeProcessor::getNodeType(typeNode)) {
			case 1: {//基本数据类型
				int tSize = atoi(
						sizeNode->getProperty("low")->mStringProperty.c_str());
				if (size + tSize > 32) {
					structMap[nodeStr] = false;
					return false;
				}
				if (tSize == 16) {
					if (size == 8) {
						structMap[nodeStr] = false;
						return false;
					} else if (size == 16) {
						size = 0;
					} else if (size == 24) {
						structMap[nodeStr] = false;
						return false;
					} else {
						size = 16;
					}
				} else if (tSize == 8) {
					size += 8;
					size %= 32;
				}
				break;
			}
			case 2: {//数组
				GNode* arrayType = getArrayTypeNode(typeNode);
				if (NodeProcessor::getNodeType(arrayType) == 3)//struct array
				{

					if (size) {
						structMap[nodeStr] = false;
						return false;
					}
					if (calStructSize(arrayType)) {
						break;
					} else {
						structMap[nodeStr] = false;
						return false;
					}
				} else {
					int arraySize = atoi(globalASTTreePointer->findNodeByIndex(
													typeNode->getProperty(
															"size")->mNodeProperty)->getProperty(
													"low")->mStringProperty.c_str());
					int arrayAlgn = atoi(fieldNode->getProperty("algn")->mStringProperty.c_str());
					if (!arraySize)//大小未确定多维数组不在结构体内计算
						break;

					switch (arrayAlgn) {
					case 32:
						if (size) {
							structMap[nodeStr] = false;
							return false;
						} else
							break;
					case 16:
						if (size == 8 || size == 24) {
							structMap[nodeStr] = false;
							return false;
						} else {
							size = (size + arraySize) % 32;
						}
						break;
					case 8:
						size = (size + arraySize) % 32;
						break;
					case 0:
						break;
					default:
						structMap[nodeStr] = false;
						return false;
					}

				}
				break;
			}
			case 3: {//结构体
				if (size) {
					structMap[nodeStr] = false;
					return false;
				}

				if (calStructSize(typeNode)) {
					break;
				} else {
					structMap[nodeStr] = false;
					return false;
				}
				break;
			}
			case 4: {//union
				int tSize = checkUnionType(typeNode);
				if (tSize == -1)
				{
					structMap[nodeStr] = false;
					return false;
				}
				size = (size + tSize) % 32;
				break;
			}
			default: {
				structMap[nodeStr] = false;
				return false;
				break;
			}
			}//switch
		}//if
		fieldNode = globalASTTreePointer->findNodeByIndex(
				fieldNode->getProperty("chan")->mNodeProperty);
	}//while
	if (size) {
		structMap[nodeStr] = false;
		return false;
	} else {
		structMap[nodeStr] = true;
		return true;
	}
}

