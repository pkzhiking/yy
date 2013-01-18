#include <stdio.h>
#include <string>
#include <iostream>
#include "include/Util.h"
#include "include/config.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sql.h>
//#include <sqlext.h>
//#include <sqltypes.h>

using std::string;
using std::cout;
using std::endl;

void outputFilesName(int iSid)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *pQuery;
    int iQueryRet; //return value of mysql_real_query

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, IP, "huawei", "huawei", "huawei", 3306, NULL, 0)){
	cout << "Error connecting to database: %s\n", mysql_error(&mysql);
	return;
    }
    cout << "successful connected." << endl;
    string strSelectSubmitUserInfo = "select * from GroupInfo, UserInfo, Submit where UserInfo.uid=Submit.uid and UserInfo.gid=GroupInfo.gid and Submit.sid=" + Util::intToString(iSid);
    pQuery = (char *) strSelectSubmitUserInfo.c_str();
    cout << pQuery << endl;
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet)
    {
	cout << "Error executing query: %s\n", mysql_error(&mysql);
	return;
    }
    cout << pQuery << " executed ..." << endl;

    res = mysql_store_result(&mysql);

    row = mysql_fetch_row(res);
    if(!row)
    {
	cout << "fail to fetch data." << endl;
	return;
    }
    string gName;
    gName = row[1];
    cout << "group name: " << gName << endl;
    string strSelectSubmit;
    strSelectSubmit = "select * from Submit where sid=" + Util::intToString(iSid);
    pQuery = (char *) strSelectSubmit.c_str();
    cout << pQuery << endl;
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet)
    {
	cout << "Error executing query: %s\n", mysql_error(&mysql);
	return;
    }
    cout << pQuery << " executed ..." << endl;

    res = mysql_store_result(&mysql);

    row = mysql_fetch_row(res);
    if(!row)
    {
	cout << "fail to fetch data." << endl;
	return;
    }

    int pid = Util::stringToInt(row[2]);
    cout << "pid: " << pid << endl;

    string strSelectProj = "select * from Project where pid=" + Util::intToString(pid);
    cout << strSelectProj << endl;
    
    pQuery = (char*) strSelectProj.c_str();
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet)
    {
	cout << "Error executing query: %s\n", mysql_error(&mysql);
	return;
    }
    cout << pQuery << " executed ..." << endl;

    
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if(!row)
    {
	cout << "fail to fetch data." << endl;
	return;
    }

    string proj_name;
    proj_name = row[1];

    cout << "porjname: " << proj_name << endl;
    

    string strSelectSubmitFile = "select * from SubmitFile where sid=" + Util::intToString((int) iSid);
    cout << strSelectSubmitFile << endl;
    pQuery = (char*) strSelectSubmitFile.c_str();
     
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet)
    {
	cout << "Error executing query: %s\n", mysql_error(&mysql);
	return;
    }
    cout << pQuery << " executed ..." << endl;

    res = mysql_store_result(&mysql);

    FILE *fp;
    fp = fopen(FILES_DIR, "w");
    if(NULL == fp)
    {
	cout << "cannot open file: " << FILES_DIR << endl;
	return ;
    }

    while(row = mysql_fetch_row(res))
    {
	string fullFileName;
	fullFileName = DEFAULT_PROJS_DIR;
	fullFileName += gName + "/" + proj_name;
	string sf_name = row[1];
	if('.' == sf_name[0])
	    sf_name = sf_name.substr(1, sf_name.size() - 1);
	fullFileName += sf_name;
	
	string old_str, new_str;
	old_str = " ";
	new_str = "\\ ";
	for(string::size_type pos(fullFileName.size() -1); string::npos != pos; pos--)
	{
	    if( string::npos != (pos = fullFileName.rfind(old_str, pos)))
		fullFileName = fullFileName.replace(pos, old_str.length(), new_str);
	    else
		break;
	}

	cout << "fullFileName: " << fullFileName << endl;
	fprintf(fp, "%s\n", fullFileName.c_str());
    }
#if 0
    string strDeleteSubmitFile = "delete from SubmitFile where sid=" + Util::intToString((int) iSid);

    cout << strDeleteSubmitFile << endl;

    pQuery = (char*) strDeleteSubmitFile.c_str();
     
    iQueryRet = mysql_real_query(&mysql, pQuery, (unsigned int) strlen(pQuery));
    if(iQueryRet)
    {
	cout << "Error executing query: %s\n", mysql_error(&mysql);
	return;
    }
    cout << pQuery << " executed ..." << endl;
#endif
    fclose(fp);
    mysql_close(&mysql);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
	cout << "please cin sid" << endl;
	return -1;
    }

    int iSid = Util::stringToInt(argv[1]);

    if(iSid < 1)
    {
	cout << "sid is not right" << endl;
	return -1;
    }
    
    FILE *fp;
    fp = fopen(FILES_DIR, "w");
    if(NULL == fp)
    {
	cout << "cannot open file: " << FILES_DIR << endl;
	return -1;
    }

    outputFilesName(iSid);

    return 0;
}
