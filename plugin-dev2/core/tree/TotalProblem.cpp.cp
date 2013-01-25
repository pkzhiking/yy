#include "../include/TotalProblem.h"
#include "../../util/include/TotalProblemHtml.h"
#include "../../util/include/Util.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <mysql/mysql.h>
#include <string.h>

using namespace std;

void ProblemClass::Print()
{
    //cout << name << ":" << count << endl;
    problem *p = head;
    for(; p; p = p->next)
    {
	p->Print();
    }
}

void ProblemList::Init()
{
    FILE *fp = freopen(PROBLEM_CONFIG, "r", stdin);
    if(NULL == fp)
    {
	//cout << "cannot open config file: " << PROBLEM_CONFIG << endl;
	return;
    }
    char tempStr[50];
    char flagOn[5];
    while(EOF != scanf("%s %s", tempStr, flagOn))
    {
	string temp(flagOn);
	if("On" == temp)
	{
	    ProblemClass *tempPC = new ProblemClass;
	    tempPC->SetCount(0);
	    tempPC->SetName(tempStr);
	    tempPC->SetLink(NULL);
	    vec_pro.push_back(*tempPC);
	}
    }
    fclose(fp);
}

void ProblemList::Add(char *prob, string fileName, int line)
{
    string str(prob);

    for(int i = 0; i < vec_pro.size(); i++)
    {
	if(str == vec_pro[i].GetName())
	{
	    struct problem *temp = new struct problem;
	    temp->fileName = fileName;
	    temp->line = line;
	    temp->next = vec_pro[i].GetHead();
	    vec_pro[i].SetHead(temp);
	    vec_pro[i].SetCount(vec_pro[i].GetCount() + 1);
	    break;
	}
    }
}

void ProblemList::Print()
{
    for(int i = 0; i < vec_pro.size(); i++)
    {
	vec_pro[i].Print();
    }
}

void ProblemList::AmendRelativeName()
{
    //string pwd(proj_name);
    //cout << "project name:" << proj_name << endl;
    for(int i = 0; i < vec_pro.size(); i++)
    {
	problem *p = vec_pro[i].GetHead();
	for(; p; p = p->next)
	{
	    //cout << "		p->fileName: " << p->fileName << endl;
	    string str_old, str_new;
	    str_old = proj_name;
	    //cout << "	    str_old.length():	    " << str_old.length() << endl;
	    str_new = ".";
	    int pos;
	    if(string::npos != (pos = p->fileName.find(str_old)))
	    {
		//cout << "pos: " << pos;
		//cout << " p->fileName: " << p->fileName;
		//cout << " str_old.length(): " << str_old.length();
		//cout << " str_new: " << str_new << endl;
		p->fileName = p->fileName.replace(pos, str_old.length(), str_new);
	    }
#if 0
	    string pwd_backup;
	    pwd_backup = pwd;
	    int pos, pos1;
	    pos = name.find('/');
	    pos1 = pwd_backup.find('/');
	    while(string::npos != pos && string::npos != pos1)
	    {
		if(pos != pos1)
		    break;
		bool quit;
		quit = false;
		for(int j = 0; j <= pos; j++)
		{
		    if(name[j] != pwd_backup[j])
		    {
			quit = true;
			break;
		    }
		}
		if(true == quit)
		    break;
		name = name.substr(pos + 1);
		pwd_backup = pwd_backup.substr(pos + 1);
		pos = name.find('/');
		pos1 = pwd_backup.find('/');
	    }
#if 1
	    string result_name;
	    result_name = "./";
	    int num = 0;
	    for(int k = 0; k < pwd_backup.size(); k++)
	    {
		if('/' == pwd_backup[k])
		    num++;
	    }
	    for(int k = 0; k < num; k++)
	    {
		result_name += "../";
	    }
	    result_name += name;
	    p->fileName = result_name;
#endif
#endif
	}
    }
}

string ProblemList::GetCommonDir(string a, string b)
{
    int pos = 1;
    int new_pos;
    bool quit = false;
    string common_dir = "/";
    while((new_pos = a.find('/', pos)) == b.find('/', pos) && new_pos > 0)
    {
	for(int i = pos; i <= new_pos; i++)
	{
	    if(a[i] != b[i])
	    {
		quit = true;
		break;
	    }
	}
	if(true == quit)
	{
	    break;
	}
	common_dir = a.substr(0, new_pos + 1);
	pos = new_pos + 1;
    }
    return common_dir;
}

void ProblemList::PrintCommonDirForDirs()
{
    for(int i = 0; i < vec_pro.size(); i++)
    {
	//cout << vec_pro[i].GetName() << endl;
    }
}

void ProblemList::DeduceProjName(int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, USER, PASSWORD, DATABASE, 3306, NULL, 0)){
	//cout << "Error connecting database: " << mysql_error(&mysql) << endl;
	return;
    }
    //cout << "Connected..." << endl;

    //get pid of the project for searching project name
    string strGetPId = "select pid from Submit where sid=" + Util::intToString(iSid);
    //cout << strGetPId << endl;
    pQuery = (char*) strGetPId.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        //cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    //cout << pQuery << "executed..." << endl;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int iPid = Util::stringToInt(row[0]);
    
    //get project name, gid according its pid
    string strGetProjName = "select name, gid from Project where pid=" + Util::intToString(iPid);
    //cout << strGetProjName << endl;
    pQuery = (char*) strGetProjName.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        //cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    //cout << pQuery << "executed..." << endl;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    string strProjName = row[0];
    int iGid = Util::stringToInt(row[1]);

    //get Group Name according its gid
    string strGetGroupName = "select name from GroupInfo where gid=" + Util::intToString(iGid);
    //cout << strGetGroupName << endl;
    pQuery = (char*) strGetGroupName.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        //cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    //cout << pQuery << "executed..." << endl;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    string strGroupName = row[0];

    proj_name = DEFAULT_PROJS_DIR + strGroupName + "/" + strProjName;
    //cout << "proj_name: " << proj_name << endl;
    mysql_close(&mysql);
}



ProblemList* ProblemList::mProblemList = NULL;

ProblemList&
ProblemList::GetInstance()
{
    if(NULL == mProblemList)
	mProblemList = new ProblemList;
    return *mProblemList;
}

void ProblemList::OutputHtml()
{
    ofstream of;
    of.open(OUTPUT_HTML);
    TotalProblemHtml tph;
    tph.ConstructHeader();
    tph.StartScript();
    tph.StartBody();
    for(int i = 0; i < vec_pro.size(); i++)
    {
	char buf[10];
	tph.ConstructD(vec_pro[i].GetName());
	tph.ConstructD2(vec_pro[i].GetName());
	tph.ConstructUse(vec_pro[i].GetName());
	sprintf(buf, "%d", vec_pro[i].GetCount());
	string temp(buf);
	string str = vec_pro[i].GetName() + ": " + temp;
	tph.AddBodyHead(str);
	problem *p = vec_pro[i].GetHead();
	tph.Span(vec_pro[i].GetName());
	tph.StartParagraph(vec_pro[i].GetName());
	for(; p; p = p->next)
	{
	    string str = p->fileName;
	    tph.ConstructBodyLine(str, p->line);
	}
	tph.EndParagraph();
	//tph.Span(vec_pro[i].GetName());
	str.clear();
	temp.clear();
    }
    tph.EndScript();
    tph.EndBody();
    tph.ConstructFooter();
    tph.Output(of);
    of.close();
}

void ProblemList::CalcDirPro()
{
    for(int i = 0; i < vec_pro.size(); i++)
    {
	problem *p = vec_pro[i].GetHead();
	for(; p; p = p->next)
	{
	    problem tempPro = *p;
	    int pos = tempPro.fileName.rfind('/');
	    string tempStr;
	    tempStr = tempPro.fileName.substr(0, pos);
	    bool flag = false, flag1 = false;
	    for(int j = 0; j < vec_dir_pro.size(); j++)
	    {
		if(vec_dir_pro[j].dirName == tempStr)
		{
		    vec_dir_pro[j].errCount++;
		    flag1 = true;
		}
		if(vec_dir_pro[j].dirName == tempPro.fileName)
		{
		    vec_dir_pro[j].errCount++;
		    flag = true;
		}
	    }
	    if(false == flag)
	    {
		DirProblem dp(tempPro.fileName);
		vec_dir_pro.push_back(dp);
	    }
	    if(false == flag1)
	    {
		DirProblem dp(tempStr);
		vec_dir_pro.push_back(dp);
	    }
	}
    }
}

void ProblemList::PrintDirPro()
{
    for(int i = 0; i < vec_dir_pro.size(); i++)
    {
	//cout << "dir " << vec_dir_pro[i].dirName << " exist " << vec_dir_pro[i].errCount << " problem" << endl;
    }
}

void ProblemList::CalcFilePro()
{
    for(int i = 0; i < vec_pro.size(); i++)
    {
	problem *p = vec_pro[i].GetHead();
	for(; p; p = p->next)
	{
	    bool flag = false;
	    for(int j = 0; j < vec_file_pro.size(); j++)
	    {
		if(vec_file_pro[j].fileName == p->fileName && vec_file_pro[j].analyzer == vec_pro[i].GetName())
		{
		    vec_file_pro[j].err_num++;
		    flag = true;
		}
	    }
	    if(false == flag)
	    {
		FileProblem fp(p->fileName, vec_pro[i].GetName(), 1);
		vec_file_pro.push_back(fp);
	    }
	    flag = false;
	}
    }
}

void ProblemList::PrintFilePro()
{
    for(int i = 0; i < vec_file_pro.size(); i++)
    {
	//cout << "file: " << vec_file_pro[i].fileName << " analyzer: " << vec_file_pro[i].analyzer << " errors: " << vec_file_pro[i].err_num << endl;
    }
}

int ProblemList::GenerateNewProjID(vector<int> vec_old_proj_id)
{
    int new_proj_id = 1;
    for(int i = 0; i < vec_old_proj_id.size(); i++)
    {
	if(vec_old_proj_id[i] >= new_proj_id)
	    new_proj_id = vec_old_proj_id[i] + 1;
    }
    return new_proj_id;
}

void ProblemList::ReportFileProToDB(int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, USER, PASSWORD, DATABASE, 3306, NULL, 0)){
	//cout << "Error connecting database: " << mysql_error(&mysql) << endl;
	return;
    }
    //cout << "Connected..." << endl;

    //insert into the table the errors for every directory
    //temp_time = Util::intToString(p->
    for(int i = 0; i < vec_dir_pro.size(); i++)
    {
	string tempStr;
	tempStr = "update SubmitFile set error_num=" + Util::intToString(vec_dir_pro[i].errCount) + " where sid=" + Util::intToString(iSid) + " and name='" + vec_dir_pro[i].dirName + "'";
	//cout << tempStr << endl;
	pQuery = (char*) tempStr.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    //cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	//cout << pQuery << "executed..." << endl;
    }

    //update the status of Submit file
    string strUpdateSubmitStatus = "update Submit set status=1 where sid=" + Util::intToString(iSid);
    //cout << strUpdateSubmitStatus << endl;
    pQuery = (char*) strUpdateSubmitStatus.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        //cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    //cout << pQuery << "executed..." << endl;

    mysql_close(&mysql);
}

void ProblemList::ReportAnalyzerProToDB(int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, USER, PASSWORD, DATABASE, 3306, NULL, 0)){
	//cout << "Error connecting database: " << mysql_error(&mysql) << endl;
	return;
    }
    //cout << "Connected..." << endl;

    for(int i = 0; i < vec_file_pro.size(); i++)
    {
	int aid;
	char analyzer_name[255];

	string strGetAnalyzerId;
	strGetAnalyzerId = "select * from Analyzer where name='" + vec_file_pro[i].analyzer + "'";
	//cout << strGetAnalyzerId << endl;
	pQuery = (char*) strGetAnalyzerId.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    //cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	//cout << pQuery << "executed..." << endl;

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	
	aid = Util::stringToInt(row[0]);
	//cout << "aid: " << aid  << endl;

	string tempStr;
	tempStr = "insert into AnalyzeResult value(" + Util::intToString(iSid) + ", '" + vec_file_pro[i].fileName + "', " + Util::intToString(aid) + ", " + Util::intToString(vec_file_pro[i].err_num) + ")";
	//cout << tempStr << vec_file_pro[i].analyzer << endl;

	pQuery = (char*) tempStr.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    //cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	//cout << pQuery << "executed..." << endl;
    }
    mysql_close(&mysql);
}

//After calculate all the errors of single files, calculate the errors of the dirs containing them
void ProblemList::ReCalFatherDirProToDB(int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, USER, PASSWORD, DATABASE, 3306, NULL, 0)){
	//cout << "Error connecting database: " << mysql_error(&mysql) << endl;
	return;
    }
    //cout << "Connected..." << endl;

    char analyzer_name[255];
    vector<pair<string, int> > vec_file_err;

    string strGetFilesError;
    strGetFilesError = "select name, error_num from SubmitFile where sid=" + Util::intToString(iSid);
    //cout << strGetFilesError << endl;
    pQuery = (char*) strGetFilesError.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        //cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    //cout << pQuery << "executed..." << endl;

    res = mysql_store_result(&mysql);

    while(NULL != (row = mysql_fetch_row(res)))
    {
	pair<string, int> p;
	p.first = row[0];
	p.second = Util::stringToInt(row[1]);
	vec_file_err.push_back(p);
	
    }

    for(int i = 0; i < vec_file_err.size(); i++)
    {
	string str_last_2_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 2, 2);

	string str_last_4_cha;
	
	if(vec_file_err[i].first.size() >= 4)
	    str_last_4_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 4, 4);
	else
	    str_last_4_cha = "dir";
	if(0 == strcmp(".c", str_last_2_cha.c_str()) || 0 == strcmp(".cpp", str_last_4_cha.c_str()))
	    continue;
	vec_file_err[i].second = 0;
	for(int j = 0; j < vec_file_err.size(); j++)
	{
	    str_last_2_cha = vec_file_err[j].first.substr(vec_file_err[j].first.size() - 2, 2);
	    if(vec_file_err[j].first.size() >= 4)
		str_last_4_cha = vec_file_err[j].first.substr(vec_file_err[j].first.size() - 4, 4);
	    else
		str_last_4_cha = "dir";
	    if(0 != strcmp(".c", str_last_2_cha.c_str()) && strcmp(".cpp", str_last_4_cha.c_str()))
		continue;
	    if(vec_file_err[j].first.size() > vec_file_err[i].first.size())
	    {
		string str_first_n = vec_file_err[j].first.substr(0, vec_file_err[i].first.size());
		if(0 == strcmp(str_first_n.c_str(), vec_file_err[i].first.c_str()))
		    vec_file_err[i].second += vec_file_err[j].second;
	    }

	}
    }

    for(int i = 0; i < vec_file_err.size(); i++)
    {
        //cout << vec_file_err[i].first << ", " << vec_file_err[i].second << endl;
    }

#if 0	
	tempStr = "insert into AnalyzeResult value(" + Util::intToString(iSid) + ", '" + vec_file_pro[i].fileName + "', " + Util::intToString(aid) + ", " + Util::intToString(vec_file_pro[i].err_num) + ")";
	cout << tempStr << vec_file_pro[i].analyzer << endl;

	pQuery = (char*) tempStr.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	cout << pQuery << "executed..." << endl;
    }
#endif
    for(int i = 0; i < vec_file_err.size(); i++)
    {
	string str_last_2_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 2, 2);

	string str_last_4_cha;
	
	if(vec_file_err[i].first.size() >= 4)
	    str_last_4_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 4, 4);
	else
	    str_last_4_cha = "dir";
	if(0 == strcmp(".c", str_last_2_cha.c_str()) || 0 == strcmp(".cpp", str_last_4_cha.c_str()))
	    continue;
	string strTemp = "update SubmitFile set error_num=" + Util::intToString(vec_file_err[i].second) + " where sid = " + Util::intToString(iSid) + " and name =\'" + vec_file_err[i].first + "\'";
	//cout << strTemp << endl;
	pQuery = (char*) strTemp.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    //cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	//cout << pQuery << "executed..." << endl;
    }
    mysql_close(&mysql);
}

//After calculate all the errors of single files, calculate the errors of the dirs containing them
void ProblemList::ReCalFatherDirProToDB(int iSid)
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

    char analyzer_name[255];
    vector<pair<string, int> > vec_file_err;

    string strGetFilesError;
    strGetFilesError = "select name, error_num from SubmitFile where sid=" + Util::intToString(iSid);
    cout << strGetFilesError << endl;
    pQuery = (char*) strGetFilesError.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet){
        cout << "Error executing query: " << mysql_error(&mysql) << endl;
        return;
    }
    cout << pQuery << "executed..." << endl;

    res = mysql_store_result(&mysql);

    while(NULL != (row = mysql_fetch_row(res)))
    {
	pair<string, int> p;
	p.first = row[0];
	p.second = Util::stringToInt(row[1]);
	vec_file_err.push_back(p);
	
    }

    for(int i = 0; i < vec_file_err.size(); i++)
    {
	string str_last_2_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 2, 2);

	string str_last_4_cha;
	
	if(vec_file_err[i].first.size() >= 4)
	    str_last_4_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 4, 4);
	else
	    str_last_4_cha = "dir";
	if(0 == strcmp(".c", str_last_2_cha.c_str()) || 0 == strcmp(".cpp", str_last_4_cha.c_str()))
	    continue;
	vec_file_err[i].second = 0;
	for(int j = 0; j < vec_file_err.size(); j++)
	{
	    str_last_2_cha = vec_file_err[j].first.substr(vec_file_err[j].first.size() - 2, 2);
	    if(vec_file_err[j].first.size() >= 4)
		str_last_4_cha = vec_file_err[j].first.substr(vec_file_err[j].first.size() - 4, 4);
	    else
		str_last_4_cha = "dir";
	    if(0 != strcmp(".c", str_last_2_cha.c_str()) && strcmp(".cpp", str_last_4_cha.c_str()))
		continue;
	    if(vec_file_err[j].first.size() > vec_file_err[i].first.size())
	    {
		string str_first_n = vec_file_err[j].first.substr(0, vec_file_err[i].first.size());
		if(0 == strcmp(str_first_n.c_str(), vec_file_err[i].first.c_str()))
		    vec_file_err[i].second += vec_file_err[j].second;
	    }

	}
    }

    for(int i = 0; i < vec_file_err.size(); i++)
    {
        cout << vec_file_err[i].first << ", " << vec_file_err[i].second << endl;
    }

#if 0	
	tempStr = "insert into AnalyzeResult value(" + Util::intToString(iSid) + ", '" + vec_file_pro[i].fileName + "', " + Util::intToString(aid) + ", " + Util::intToString(vec_file_pro[i].err_num) + ")";
	cout << tempStr << vec_file_pro[i].analyzer << endl;

	pQuery = (char*) tempStr.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	cout << pQuery << "executed..." << endl;
    }
#endif
    for(int i = 0; i < vec_file_err.size(); i++)
    {
	string str_last_2_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 2, 2);

	string str_last_4_cha;
	
	if(vec_file_err[i].first.size() >= 4)
	    str_last_4_cha = vec_file_err[i].first.substr(vec_file_err[i].first.size() - 4, 4);
	else
	    str_last_4_cha = "dir";
	if(0 == strcmp(".c", str_last_2_cha.c_str()) || 0 == strcmp(".cpp", str_last_4_cha.c_str()))
	    continue;
	string strTemp = "update SubmitFile set error_num=" + Util::intToString(vec_file_err[i].second) + " where sid = " + Util::intToString(iSid) + " and name =\'" + vec_file_err[i].first + "\'";
	cout << strTemp << endl;
	pQuery = (char*) strTemp.c_str();
	iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
	if(iQueryRet){
	    cout << "Error executing query: " << mysql_error(&mysql) << endl;
	    return;
	}
	cout << pQuery << "executed..." << endl;
    }
    mysql_close(&mysql);
}
