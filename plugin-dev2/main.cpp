/*
 * main.cpp
 *
 *  Created on: 2012-7-17
 *      Author: administrator
 */
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <iostream>
#include <time.h>
#include <mysql/mysql.h>

#include "core/include/ASTTreeRebuild.h"
#include "core/include/TreePathWalker.h"
#include "core/include/AnalyzerManager.h"
#include "core/include/config.h"
#include "core/include/ReportManager.h"
#include "core/include/VarDeclCollector.h"
#include "core/include/TotalProblem.h"
#include "core/include/ConfigManager.h"
#include "util/include/Util.h"

#include "analyzer/include/DivideByTwosExponentAnalyzer.h"
#include "analyzer/include/MemApplyAnalyzer.h"
#include "analyzer/include/StructSizeAnalyzer.h"
#include "analyzer/include/BadMemOperationCallAnalyzer.h"
#include "analyzer/include/InitializeArrayByForLoopAnalyzer.h"
#include "analyzer/include/AssignToConstantAnalyzer.h"
#include "analyzer/include/MultiplyByTwosExponentAnalyzer.h"
#include "analyzer/include/GeAndLeCondAnalyzer.h"
#include "analyzer/include/SqrtAnalyzer.h"
#include "analyzer/include/RealDivisionAnalyzer.h"
#include "analyzer/include/GlobalVarSizeAnalyzer.h"
#include "analyzer/include/BitNotExprAnalyzer.h"
#include "analyzer/include/ForLoopToZeroAnalyzer.h"
#include "analyzer/include/ForLoopCombineAnalyzer.h"
#include "analyzer/include/FunctionCallAnalyzer.h"
#include "analyzer/include/StringCopyAnalyzer.h"
#include "analyzer/include/IfToSwitchAnalyzer.h"
#include "analyzer/include/FunctionAsLoopVarAnalyzer.h"
#include "analyzer/include/LoopDivideAnalyzer.h"
#include "analyzer/include/LocalVarSizeAnalyzer.h"
#include "analyzer/include/FunctionCodeSizeAnalyzer.h"
#include "analyzer/include/CalcMemAnalyzer.h"
#include "analyzer/include/IfToIfelseAnalyzer.h"
#include "analyzer/include/ReducibleIfAnalyzer.h"
#include "analyzer/include/ConditionInLoopAnalyzer.h"
#include "analyzer/include/FindSameFunctionAnalyzer.h"
#include "analyzer/include/FloatTriCallAnalyzer.h"
#include "analyzer/include/FunctionParameterAnalyzer.h"
#include "analyzer/include/MultiConditionAnalyzer.h"
#include "analyzer/include/StructPrmsAnalyzer.h"

#include "util/include/TotalProblemHtml.h"

AnalyzerManager &manager = AnalyzerManager::getInstance();
GTree *globalASTTreePointer;

void analyzerDir(string dirName)
{

	struct dirent* ent = NULL;
	 DIR *pDir;

		if (dirName[dirName.size()-1] != '/'){
			dirName += "/";
		}

		if ((pDir = opendir(dirName.c_str())) == NULL){
			cerr << "Can't open Directory " << dirName << endl;
			return;
		}

	while (NULL != (ent = readdir(pDir))){
	  struct stat buf;
	  string current_file;
	  current_file = dirName + ent->d_name;
	  if (lstat(current_file.c_str(), &buf) < 0)
	  {
	      cout << "error reading dir." << endl;
	      continue;
	  }
	  if(S_ISREG(buf.st_mode)){
	   //file
	   string functionName(ent->d_name);
	   if(functionName.find(".") == string::npos){
	    	  continue;
	       }
	   ASTTreeRebuild *functionTree = new ASTTreeRebuild(dirName + functionName);
	   globalASTTreePointer = functionTree->ASTTreePointer;

	   //cout << functionTree->ASTTreePointer->_getFullFileName() << "----" << functionName << endl;

	   TreePathWalker walker(functionTree->ASTTreePointer);
	   SrcManager::getInstance().init(functionTree->ASTTreePointer->_getFullFileName());
	   walker.startTreeWalk();
	   //todo:change location
	  // ReportManager::getInstance().flushReportToFileSystem("/home/dandelion/plugin-dev2/run-result/");
	   manager.clearAnalyzerState();


           if(Config::GetInstance().IsOn("FindSameFunctionAnalyzer"))
	       FindSameFunctionAnalyzer::getInstance().globalAnalyze(functionTree->ASTTreePointer->_getHashCode(), dirName + functionName);

	   delete functionTree;
	   delete globalASTTreePointer;
		  }
	  else if (S_ISDIR(buf.st_mode) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
		  	  	  analyzerDir(dirName + ent->d_name);
	  	   }
	  }
}

//initialize analyzer table through looking up database
void initWebAnalyzer(map<string, bool> &map_WebAnalyzerOn, int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, USER, PASSWORD, DATABASE, 3306, NULL, 0)){
	cout << "Error connecting database: " << mysql_error(&mysql) << endl;
	return;
    }
    cout << "Connected..." << endl;

    //insert into the table the errors for every directory
    //temp_time = Util::intToString(p->
    string strLoopUpAnalyzer;
    strLoopUpAnalyzer = "select analyzers from Submit where sid=" + Util::intToString(iSid);
    cout << strLoopUpAnalyzer << endl;
    pQuery = (char*) strLoopUpAnalyzer.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    string strTemp = row[0];
    string strGetAnalyzerStatus = "select name from Analyzer order by aid";
    cout << strGetAnalyzerStatus << endl;
    pQuery = (char*) strGetAnalyzerStatus.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    res = mysql_store_result(&mysql);
    int i = 0;
    while( row = mysql_fetch_row(res))
    {
	string strAnalyzer = row[0];
	map_WebAnalyzerOn.insert(map<string, bool>::value_type(strAnalyzer, (bool) (strTemp[i++] - '0') ));
    }

    for(map<string, bool>::iterator iter = map_WebAnalyzerOn.begin(); iter != map_WebAnalyzerOn.end(); iter++)
    {
	cout << iter->first << " On:" << iter->second << endl;
    }

    mysql_free_result(res);
    mysql_close(&mysql);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
	cout << "please cin sid" << endl;
	return -1;
    }
    map<string, bool> map_WebAnalyzerOn;
    initWebAnalyzer(map_WebAnalyzerOn, Util::stringToInt(argv[1]));
	  	Config::GetInstance().Init();
			
			//manager.addAnalyzer(new VarDeclCollector());
			map<string, bool>::iterator iter;
			bool status = 0;
			iter = map_WebAnalyzerOn.find("DivideByTwosExponentAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("DivideByTwosExponentAnalyzer") && status)
			    manager.addAnalyzer(new DivideByTwosExponentAnalyzer());

			iter = map_WebAnalyzerOn.find("MemApplyAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("MemApplyAnalyzer") && status)
			    manager.addAnalyzer(new MemApplyAnalyzer());

			iter = map_WebAnalyzerOn.find("StructSizeAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("StructSizeAnalyzer") && status)
			    manager.addAnalyzer(new StructSizeAnalyzer());

			iter = map_WebAnalyzerOn.find("BadMemOperationCallAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("BadMemOperationCallAnalyzer") && status)
			    manager.addAnalyzer(new BadMemOperationCallAnalyzer());

			iter = map_WebAnalyzerOn.find("AssignToConstantAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("AssignToConstantAnalyzer") && status)
			    manager.addAnalyzer(new AssignToConstantAnalyzer());

			iter = map_WebAnalyzerOn.find("InitializeArrayByForLoopAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("InitializeArrayByForLoopAnalyzer") && status)
			    manager.addAnalyzer(new InitializeArrayByForLoopAnalyzer());

			iter = map_WebAnalyzerOn.find("MultiplyByTwosExponentAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("MultiplyByTwosExponentAnalyzer") && status)
			    manager.addAnalyzer(new MultiplyByTwosExponentAnalyzer());

			iter = map_WebAnalyzerOn.find("GeAnaLeCondAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("GeAndLeCondAnalyzer") && status)
			    manager.addAnalyzer(new GeAndLeCondAnalyzer());

#if 1
			iter = map_WebAnalyzerOn.find("SqrtAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("SqrtAnalyzer") && status)
			    manager.addAnalyzer( new SqrtAnalyzer() );
#endif

			iter = map_WebAnalyzerOn.find("RealDivisionAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("RealDivisionAnalyzer") && status)
			    manager.addAnalyzer( new RealDivisionAnalyzer() );

			iter = map_WebAnalyzerOn.find("GlobalVarSizeAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("GlobalVarSizeAnalyzer") && status)
			    manager.addAnalyzer( new GlobalVarSizeAnalyzer() );

			iter = map_WebAnalyzerOn.find("BitNotExprAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("BitNotExprAnalyzer") && status)
			    manager.addAnalyzer( new BitNotExprAnalyzer() );

			iter = map_WebAnalyzerOn.find("ForLoopToZeroAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("ForLoopToZeroAnalyzer") && status)
			    manager.addAnalyzer( new ForLoopToZeroAnalyzer() );

			iter = map_WebAnalyzerOn.find("ForLoopCombineAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("ForLoopCombineAnalyzer") && status)
			    manager.addAnalyzer( new ForLoopCombineAnalyzer() );

			iter = map_WebAnalyzerOn.find("FunctionCallAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("FunctionCallAnalyzer") && status)
			    manager.addAnalyzer( new FunctionCallAnalyzer());

			iter = map_WebAnalyzerOn.find("StringCopyAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("StringCopyAnalyzer")  && status)
			    manager.addAnalyzer( new StringCopyAnalyzer());

			iter = map_WebAnalyzerOn.find("IfToSwitchAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("IfToSwitchAnalyzer") && status)
			    manager.addAnalyzer( new IfToSwitchAnalyzer());

			iter = map_WebAnalyzerOn.find("FunctionAsLoopVarAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("FunctionAsLoopVarAnalyzer") && status)
			    manager.addAnalyzer( new FunctionAsLoopVarAnalyzer());

			iter = map_WebAnalyzerOn.find("LoopDivideAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("LoopDivideAnalyzer") && status)
			    manager.addAnalyzer(new LoopDivideAnalyzer());

			iter = map_WebAnalyzerOn.find("FunctionCodeSizeAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("FunctionCodeSizeAnalyzer") && status)
			    manager.addAnalyzer(new FunctionCodeSizeAnalyzer());

			iter = map_WebAnalyzerOn.find("LocalVarSizeAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("LocalVarSizeAnalyzer") && status)
			    manager.addAnalyzer(new LocalVarSizeAnalyzer());
#if 1
			iter = map_WebAnalyzerOn.find("IfToIfelseAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("IfToIfelseAnalyzer") && status)
			    manager.addAnalyzer(new IfToIfelseAnalyzer());

			iter = map_WebAnalyzerOn.find("ReducibleIfAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("ReducibleIfAnalyzer") && status)
			    manager.addAnalyzer(new ReducibleIfAnalyzer());
#endif

			iter = map_WebAnalyzerOn.find("ConditionInLoopAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("ConditionInLoopAnalyzer") && status)
			    manager.addAnalyzer(new ConditionInLoopAnalyzer());
#if 1
			iter = map_WebAnalyzerOn.find("FloatTriCallAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("FloatTriCallAnalyzer") && status)
			    manager.addAnalyzer(new FloatTriCallAnalyzer());

			iter = map_WebAnalyzerOn.find("FunctionParameterAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("FunctionParameterAnalyzer") && status)
			    manager.addAnalyzer(new FunctionParameterAnalyzer());

			iter = map_WebAnalyzerOn.find("MultiConditionAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("MultiConditionAnalyzer") && status)
			    manager.addAnalyzer(new MultiConditionAnalyzer());
#endif

			iter = map_WebAnalyzerOn.find("StructPrmsAnalyzer");
			status = iter->second;
			if(Config::GetInstance().IsOn("StructPrmsAnalyzer") && status)
			    manager.addAnalyzer(new StructPrmsAnalyzer());
			//if(Config::GetInstance().IsOn("FindSameFunctionAnalyzer"))

			
			//manager.addAnalyzer(new CalcMemAnalyzer());

	 ProblemList::GetInstance().Init();

	 analyzerDir(INPUT_FOLD);
	 

	 ReportManager::getInstance().flushReportToFileSystem(REPORT_FOLD);

	 ProblemList::GetInstance().DeduceProjName(Util::stringToInt(argv[1]));

	 ProblemList::GetInstance().Print();

	 cout << "	AMENDRELATIVENAME	" << endl;
	 ProblemList::GetInstance().AmendRelativeName();
	 cout << "	print after amending	    " << endl;
	 ProblemList::GetInstance().Print();

	 ProblemList::GetInstance().PrintCommonDir();
	 ProblemList::GetInstance().PrintCommonDirForDirs();
	 //ProblemList::GetInstance().Print();
#if 1
	 ProblemList::GetInstance().OutputHtml();
	 ProblemList::GetInstance().CalcDirPro();
	 ProblemList::GetInstance().CalcFilePro();
	 ProblemList::GetInstance().ReportFileProToDB(Util::stringToInt(argv[1]));
	 ProblemList::GetInstance().ReportAnalyzerProToDB(Util::stringToInt(argv[1]));
	 ProblemList::GetInstance().ReCalFatherDirProToDB(Util::stringToInt(argv[1]));
	 ProblemList::GetInstance().UpdateSubmitToDB(Util::stringToInt(argv[1]));
	 return 1;
#endif
}



