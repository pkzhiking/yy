#ifndef TOTALPROBLEM_H
#define TOTALPROBLEM_H

#include <vector>
#include <iostream>
#include <time.h>

#include "config.h"

using namespace std;

//a problem created by scanning a programmer's source code, so the user name is added to be tackled later
struct problem
{
    string fileName;
    int line;
    string user;
    problem *next;
    void Print(){cout<< "\t" << fileName << "\t" << line << endl;}
};

//problem of a directory include all problems under it
struct DirProblem
{
    string dirName;
    //string dirType;
    unsigned int errCount;
    DirProblem(){errCount = 0;}
    DirProblem(string str, unsigned int err = 1){dirName = str; errCount = err;}
};

//problem of a file, include all analyzers' problems
struct FileProblem
{
    string fileName;
    string analyzer;
    string user;
    unsigned int err_num;
    FileProblem(string name, string ana, unsigned  int num){fileName = name; analyzer = ana; err_num = num;}

    bool operator ==(FileProblem& fp)
    {
	if(fp.fileName == fileName && fp.analyzer == analyzer && fp.err_num == err_num)
	    return true;
	return false;
    }

    bool operator !=(FileProblem& fp)
    {
	if(fp.fileName != fileName || fp.analyzer != analyzer)
	    return true;
	return false;
    }

    bool operator ^=(FileProblem& fp)
    {
	if(fp.fileName == fileName && fp.analyzer == analyzer && fp.err_num != err_num)
	    return true;
	return false;
    }
};

struct FileDiff
{
    string fileName;
    string analyzer;
    unsigned int original;
    unsigned int current;
    void Print()
    {
	cout << "file: " << fileName << " analyzer: " << analyzer << " original: " << original << " current: " << current << endl;
    }
};


class ProblemClass
{
private:
    int count;
    string name;
    problem *head;
public:
    int GetCount(){return count;}
    void SetCount(int i){count = i;}
    void SetLink(void *p){head = (problem*) p;}
    void Print();
    problem* GetHead(){return head;}
    void SetHead(problem *h){head = h;}
    string GetName(){return name;}
    void SetName(string str){name = str;}
};

class ProblemList
{
private:
    vector<ProblemClass> vec_pro;
    static ProblemList* mProblemList; 
    vector<DirProblem> vec_dir_pro;
    vector<FileProblem> vec_file_pro;
    string proj_name;
    int proj_id;
public:
    void Init();
    void Add(char *, string, int);
    string GetCommonDir(string, string);
    void Print();
    void SetID(int id){proj_id = id;}
    int GetID(){return proj_id;}
    int GenerateNewProjID(vector<int>);
    void SetProjName(string name){proj_name = name;}
    string GetProjName(){return proj_name;}
    void DeduceProjName(int);
    static ProblemList& GetInstance(); 
    void OutputHtml();
    void ReportFileProToDB(int);
    void CalcDirPro();
    void CalcFilePro();
    void PrintDirPro();
    void PrintFilePro();
    void ReportAnalyzerProToDB(int);
    void AmendRelativeName();
    void PrintCommonDir(){cout << proj_name << endl;}
    void PrintCommonDirForDirs();
    void ReCalFatherDirProToDB(int);
    void UpdateSubmitToDB(int);
};

#endif
