/*
 * mem_test.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: Guo Jiuliang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

//void strcpy(char*,char*);
//void strncpy(char*,char*,int n);
char ccc[100] = "ttttt";
struct simplepod
{
    int x;
    int y;
};
struct simple_nonpod
{
	int* p;
	double x;
	float y;
};
struct non_pod_struct
{
    char p;
    static const int x = 10;
    double y;
    int *intarray;
    double *doublearray;
    int z;
    simplepod pod_mem;
};
struct podStruct
{
    char x;
    int v;
    static const int var = 1024;
    double item;
    double radius;
    double length;
    float v1;
    simplepod x2;
};
struct nonPOD_with_nonPOD_mem
{
	int p;
	double v;
	simplepod POD_mem;
	simple_nonpod nonPOD_mem;
};
non_pod_struct globalvar;
void mem_test()
{
	int intVar1;
	double doubleVar2;
	int *ptr_intVar1 = &intVar1;
	double *ptr_doubleVar2 = &doubleVar2;

	memset(&intVar1,0,100);
	memset(&doubleVar2,0,100);
	memset(ptr_intVar1,0,100);
	memset(ptr_doubleVar2,0,100);
	memset(&globalvar.y,0,100);
	memset(&globalvar,0,100);
	//--------------------------
}
void pod_test()
{
	struct nonPOD_with_nonPOD_mem nonPOD_var;
	struct podStruct pod_var;
	struct nonPOD_with_nonPOD_mem *nonPOD_ptr;
	struct podStruct *pod_ptr;
	//-----------------------------
	memset(&nonPOD_var,0,100); 					//should't found
	memset(&pod_var,0,100); 					//should found
	memset(nonPOD_ptr,0,100); 					//should't found
	memset(pod_ptr,0,100);   					//should found
	memset(&nonPOD_var.POD_mem,0,100); 			//should found
	memset(&nonPOD_var.nonPOD_mem,0,100); 		//should't found
	memset(&(nonPOD_ptr->POD_mem),0,100);		//should found
	memset(&(nonPOD_ptr->nonPOD_mem),0,100); 	//should't found
}

