#include "include/ConfigManager.h"
#include "include/config.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

Config::Config()
{
}

Config::~Config()
{
}

Config* Config::cPConfig = NULL;

void Config::Init()
{
    FILE *fp = freopen(PROBLEM_CONFIG, "r", stdin);
    if(NULL == fp)
    {
	cout << "cannot open file: " << PROBLEM_CONFIG << endl;
	return;
    }
    char name[50];
    char flagOn[10];
    ConfigUnit temp;
    while(-1 != scanf("%s %s", name, flagOn))
    {
	temp.name = name;
	temp.flagOn = flagOn;
	vec_Conf.push_back(temp);
    }
    fclose(fp);
}

bool Config::IsOn(string str)
{
    for(int i = 0; i < vec_Conf.size(); i++)
    {
	if(str == vec_Conf[i].name && "On" == vec_Conf[i].flagOn)
	{
	    return true;
	}
    }
    return false;
}

Config&
Config::GetInstance()
{
    if(NULL == cPConfig)
	cPConfig = new Config;
    return *cPConfig;
}
