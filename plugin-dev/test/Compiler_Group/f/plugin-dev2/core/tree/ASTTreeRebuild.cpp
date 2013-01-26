
//#include "config.h"
#include "../include/ASTTreeRebuild.h"
#include "../include/ASTTree.h"
#include "../../util/include/Logger.h"
#include "../include/config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <stdio.h>
#include "../include/FunctionIndex.h"
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using std::cin;

ASTTreeRebuild::ASTTreeRebuild(string fileName)
{
	 init(fileName);
}
ASTTreeRebuild::ASTTreeRebuild(string fileName, string functionName)
{

	char str[256];
	strcpy(str,fileName.c_str());
	strcat(str, "$&$");
	strcat(str, functionName.c_str());

	string loc = FunctionIndex::getInstance().getLoction(str);
	init(loc);
}
ASTTreeRebuild::~ASTTreeRebuild()
{
}
void
ASTTreeRebuild::init(string fileName)
{

		if (freopen(fileName.c_str(), "r", stdin) == NULL)
		{
			//TODO :
			this->ASTTreePointer = NULL;
			return ;
		}
		this->ASTTreePointer = new GTree();
		char str[256];
		gets(str);
		this->ASTTreePointer->_setFullFileName(str);

		unsigned int nodeID;

		while(scanf("%d %s", &nodeID, str) != EOF)
		{
			//	printf("@%d %s\n", nodeID, name);
				GNode* newNode = new GNode();
				newNode->_addTreeNode(nodeID, str);

	//			int childNum;
	//			scanf("%*s %d", childNum);
	//			for(int i = 0; i < childNum; i++)
	//			{
	//				int childID;
	//				scanf("%d", childID);
	//				newNode->_addChild(childID);
	//			}
				char temp[256];
				int propertyNum;
				scanf("%*s %d", &propertyNum);
		//		printf("%s %d\n", "propertyNum", propertyNum);
				gets(temp);
				for(int i = 0; i < propertyNum; i++)
				{
					GProperty prop;
					string propName;
					int type;
					scanf("%d", &type);
					gets(temp);
					gets(temp);
				// printf("%d @%s@ ", type, temp);
					propName = temp;
					if (type == 0)
					{
						prop.ePropertyType = GProperty::PROP_STRING_TYPE;
						gets(temp);
						prop.mStringProperty = temp;
					//	printf("%s\n", temp);
					}else
					{
						prop.ePropertyType = GProperty::PROP_INDEX_TYPE;
						scanf("%d", &prop.mNodeProperty);
					//	printf("%d\n", prop.mNodeProperty);
					}
					newNode->_addPropery(propName, prop);
				}

				this->ASTTreePointer->addTreeNode(newNode);

		}
		this->ASTTreePointer->_setHashCode(nodeID);
}
