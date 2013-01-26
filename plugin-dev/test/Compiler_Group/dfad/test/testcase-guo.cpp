/*
 * testcase-guo.cpp
 *
 *  Created on : May 6, 2012
 *      Author : Guo Jiuliang
 *  Description: This file mainly contains test cases for my own use
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
//very simple POD type to put into another struct to test nested struct
struct simplepod
{
    int x;
    int y;
};
//very simple non POD type to put into another struct to test nested struct
struct simple_nonpod
{
	int* p;
	double x;
	float y;
};
//non POD type with nested POD type
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
//nested POD type struct
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
//non POD type with not pod nested member
struct nonPOD_with_nonPOD_mem
{
	int p;
	double v;
	simplepod POD_mem;
	simple_nonpod nonPOD_mem;
};
struct complexPOD{
	int x;
	podStruct y;
};
struct complexNonPOD{
	int x;
	nonPOD_with_nonPOD_mem y;
};
non_pod_struct globalvar;
//BadMemOperationCallAnalyzer
void pod_test()
{
	struct nonPOD_with_nonPOD_mem nonPOD_var;
	struct podStruct pod_var;
	struct nonPOD_with_nonPOD_mem *nonPOD_ptr;
	struct podStruct *pod_ptr;
	struct complexPOD compPODVar1;
	struct complexNonPOD compNonPODVar2;
	//-----------------------------
	memset(&nonPOD_var,0,100); 					//should't find
	memset(&pod_var,0,100); 					//should find
	memset(nonPOD_ptr,0,100); 					//should't find
	memset(pod_ptr,0,100);   					//should find
	memset(&nonPOD_var.POD_mem,0,100); 			//should find
	memset(&nonPOD_var.nonPOD_mem,0,100); 		//should't find
	memset(&(nonPOD_ptr->POD_mem),0,100);		//should find
	memset(&(nonPOD_ptr->nonPOD_mem),0,100); 	//should't find
    //-------------------------------
	//test complex struct with multi-level nested members
	memset(&compPODVar1.y.x2,0,100);            //should find
	memset(&compNonPODVar2.y.POD_mem,0,100);    //should find
	memset(&compNonPODVar2.y.nonPOD_mem,0,100); //should't find
}
//MultiplyByTwosExponentAnalyzer
//DivideByTwosExponentAnalyzer
void div_mul_2s_expo_test()
{
	const int c1 = 128;
    int x1 = 32;
    int y = 100/x1;
    int n = 100/c1;	   //should find
    int z = x1/64;    //should find
	int x = 32;
	int y2 = 100 / x;
	int n2 = 100 * x;
	int z2 = x / 64;  //should find
	x = y * 128;      //should find
	z = y * 133;
}
//InitializeArrayByForLoopAnalyzer
void using_for_loop_to_init_array_to_const()
{
	int *ptr2 = new int [100];
	for(int i=0;i<100;++i)
	{
		ptr2[i] = 0x0;
	}
}

