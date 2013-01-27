#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
using std::sqrt;
struct cstruct {
	char p;
	static const int x = 10;
	double y;
	int *intarray;
	double *doublearray;
	int z;
};
void my_memcpy(void*,int value,int num);
void strncpy(char*,char*,int n);
struct simplepod {
	static const int x = 10;
	static const int y = 10;
	static const int z = x / 3;
};
struct empty {
	char tt;
	short int ttt;
};
struct podStruct {
	union {
		int a;
		char d;
		char k[6];
	} ttt;
	empty x2[4];
	char t3;
	int v;
	static int var;
	double item;
	double radius;
	double length;
	float v1;

};
struct nn {
	char a;
	empty tt;
};
struct ok {
	char aaa[6][4];
	static int x;
	int a;
	char av[4];
	short int aa;
	char b[7][4];
	short int ak;
};
cstruct globalvar;
void test_record()
{
	struct podStruct pod_var;
	struct cstruct non_pod_var;
}
/*
int test_stack_simple()
{
    int intVar1;
    int intVar2;
}
*/
/*
int test_stack_full()
{
    int intVar1;
    int intVar2;
    if(intVar1>0)
    {
        int arrVar1[10];
    }
    else
    { if(intVar2>10)
        {
            double doubleArrVar1[20]; 
        }
        else
        {
            float floatArrVar2[33]; 
        }
        int var2[20];
    }
    return 0;
}
*/
int func5() {
	memset(&globalvar,0,100);
	ok tok;
	podStruct testPodVar;
	simplepod test1;
	nn test2;
	ok tok1;
	memset(&globalvar,0,100);
	//nn t2;
	//cstruct test1;
	return 0;

}
#define VOS_MemAlloc(size) (char *)malloc(size)
void test() {
	memset(&globalvar,0,100);
	ok tok;
	nn test2;
	podStruct testPodVar1;
	char *str;
	int i;
	for (i = 0; i < 10; i++) {
		VOS_MemAlloc(10);
	}
	while (true)
		VOS_MemAlloc(10);
	for (i = 1; i < 10; i++) {
		if (i > 5) {
			VOS_MemAlloc(10);
		} else {
			while (true)
				VOS_MemAlloc(10);
		}
	}
int intVar1 = 0x16;
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
int testBitNot = ~intVar1;
for ( intVar1 = 0; intVar1 == 0 && intVar1 != 111 || intVar1 ==3213; intVar1++ )
	testBitNot++;
for ( intVar1 = 0; intVar1 == 0 && intVar1 != 111 || intVar1 ==3213; intVar1++ )
        testBitNot++;
double a = 1.0;
double b = sqrt( (double)a );
double c = a / 2;
c = a / 2.3;
b = a / c;
float fa;
fa = fa / 2;
fa = fa / 2.2;
float fb;
fb = fb / fa;
    int xxxx;
    const int ycst = 20;
    int *yptr; //var_decl pointer_type
    yptr = &xxxx;
    int intVar = 0;
    float floatVar = 0.0f;
    double doubleVar = 0.0;
    strncpy(NULL,NULL,xxxx); //var_decl
    strncpy(NULL,NULL,globalvar.x); //component_ref
    cstruct * structPtr = &globalvar;
    strncpy(NULL,NULL,structPtr->z);
    strncpy(NULL,NULL,10);
    strncpy(NULL,NULL,ycst);
    strncpy(NULL,NULL,structPtr->x);
    doubleVar = 0.2;
    podStruct testPodVar;
    my_memcpy(&testPodVar,NULL,10);
    char *arr1 = new char[100];
    char arr2[100];
    const int con_int = 0;
    for(int i=0;i<100;++i)
    {
        arr1[i] = 0;
    }
    cstruct structVar;
    globalvar.intarray = NULL;
    int *ptr2 = new int [100];
    for(int i=0;i<100;++i)
    {
        ptr2[i] = 0x0;
    }
    int p;
    p = 0x0;
    char n;
    strcpy(NULL,NULL);
    int value_pred = 100;
    if(value_pred>10)
    {
        int def_var1 = 1;
        while(def_var1>=1 && (def_var1<=1 || value_pred>10))
            {
                strncpy(NULL,NULL,100);
                for(int index = 0;index<10;++index)
                {
                    my_memcpy(&globalvar.p,'-',4);
                }
            }
    int x = 32;
    int y = 100/x;
    int n = 100/x;
    int z = x/64;
    }
    else
    {
        strcpy(NULL,NULL);
        int def_var2 = 1;
    }
    while(true)
    {
                for(int index = 0;index<10;++index)
                {
                    //my_memcpy(NULL,NULL);
                    for(int j=0;j<100;++j)
                    {
                        strncpy(NULL,NULL,j);
                    }
                }
                //my_memcpy(NULL,NULL);
    }
}

int test_stack_simple() {
	int intVar1;
	int intVar2;
	nn test2;
	podStruct testPodVar1;
	return 0;
}
int test_stack_full() {
	int intVar1;
	int intVar2;
	if (intVar1 > 0) {
		int arrVar1[10];
	} else {
		if (intVar2 > 10) {
			double doubleArrVar1[20];
		} else {
			float floatArrVar2[33];
		}
		int var2[20];
	}
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
VOS_MemAlloc(10);
return 0;
}
int test_stack() {
	int x;
	if (x > 0) {
		int var1[10];
	} else {
		int var2[20];
	}
	return 0;
}

void test1() {
	char *str;
	int i;
	for (i = 0; i < 100; i++) {
		VOS_MemAlloc(10);
	}
	while (true)
		VOS_MemAlloc(10);
	for (i = 1; i < 10; i++) {
		if (i > 5) {
			VOS_MemAlloc(10);
		} else {
			while (true)
				VOS_MemAlloc(10);
		}
	}
}
int func2()
{
    globalvar.intarray = NULL;
    int *ptr2 = new int [100];
    for(int i=0;i<100;++i)
    {
        ptr2[i] = 0x0;
    }
    int p;
    p = 0x0;
    char n;
 //   strcpy(NULL,NULL);
    int value_pred = 100;
    if(value_pred>10)
    {
        int def_var1 = 1;
        while(def_var1>=1 && (def_var1<=1 || value_pred>10))
            {
  //              strncpy(NULL,NULL,100);
                for(int index = 0;index<10;++index)
                {
                    //my_memcpy(&globalvar.p,'-',4);
                }
            }
    int x = 32;
    int y = 100/x;
    int n = 100/x;
    int z = x/64;
    }
    else
    {
//        strcpy(NULL,NULL);
        int def_var2 = 1;
    }
    while(true)
    {
                for(int index = 0;index<10;++index)
                {
                    //my_memcpy(NULL,NULL);
                    for(int j=0;j<100;++j)
                    {
                        strncpy(NULL,NULL,j);
                    }
                }
                //my_memcpy(NULL,NULL);
    }
}
/*
int func1() {
	int intVar1;
	int intVar2;
	if (intVar1 > 0) {
		int arrVar1[10];
	} else {
		if (intVar2 > 10) {
			double doubleArrVar1[20];
		} else {
			float floatArrVar2[33];
		}
		int var2[20];
	}
	memset(&intVar1, 100, 10);
	int xxxx;
	const int ycst = 20;
	int *yptr; //var_decl pointer_type
	yptr = &xxxx;
	int intVar = 0;
	float floatVar = 0.0f;
	double doubleVar = 0.0;
	strncpy(NULL, NULL, xxxx); //var_decl
	strncpy(NULL, NULL, globalvar.x); //component_ref
	cstruct * structPtr = &globalvar;
	strncpy(NULL, NULL, structPtr->z);
	strncpy(NULL, NULL, 10);
	strncpy(NULL, NULL, ycst);
	strncpy(NULL, NULL, structPtr->x);
	doubleVar = 0.2;
	podStruct testPodVar;
	char *arr1 = new char[100];
	char arr2[100];
	const int con_int = 0;
	for (int i = 0; i < 100; ++i) {
		arr1[i] = 0;
	}
	cstruct structVar;
	globalvar.intarray = NULL;
	int *ptr2 = new int[100];
	for (int i = 0; i < 100; i) {
		ptr2[i] = 0x0;
		intVar = 1001;
	}
	memset(ptr2, 0, 100);
	memset(&globalvar.y, 5, 3);
	int p;
	p = 0x0;
	char n;
	strcpy(NULL, NULL);
	int value_pred = 100;
	memset(&doubleVar, 0, 4);
	if (value_pred > 10) {
		int def_var1 = 1;
		while (def_var1 >= 1 && (def_var1 <= 1 || value_pred > 10)) {
			strncpy(NULL, NULL, 100);
			for (int index = 0; index < 10; ++index) {
			}
		}
		int x = 32;
		int y = 100 / x;
		int n = 100 / x;
		int z = x / 64;
		x = y * 128;
		z = y * 133;
	} else {
		strcpy(NULL, NULL);
		int def_var2 = 1;
	}
	memset(structPtr, 10, 5);
	while (true) {
		for (int index = 0; index < 10; ++index) {
			//my_memcpy(NULL,NULL);
			for (int j = 0; j < 100; ++j) {
				strncpy(NULL, NULL, j);
			}
		}
		//my_memcpy(NULL,NULL);

	}
	//my_memcpy(NULL,NULL);
	return 100;

}

int func2() {
	globalvar.intarray = NULL;
	int *ptr2 = new int[100];
	for (int i = 0; i < 100; ++i) {
		ptr2[i] = 0x0;
	}
	int p;
	p = 0x0;
	char n;
	//   strcpy(NULL,NULL);
	int value_pred = 100;
	if (value_pred > 10) {
		int def_var1 = 1;
		while (def_var1 >= 1 && (def_var1 <= 1 || value_pred > 10)) {
			//              strncpy(NULL,NULL,100);
			for (int index = 0; index < 10; ++index) {
				//my_memcpy(&globalvar.p,'-',4);
			}
		}
		int x = 32;
		int y = 100 / x;
		int n = 100 / x;
		int z = x / 64;
	} else {
		//        strcpy(NULL,NULL);
		int def_var2 = 1;
	}
	while (true) {
		for (int index = 0; index < 10; ++index) {
			//my_memcpy(NULL,NULL);
			for (int j = 0; j < 100; ++j) {
				//                       strncpy(NULL,NULL,j);
			}
		}
		//my_memcpy(NULL,NULL);

	}
	//my_memcpy(NULL,NULL);
	return 100;
}
*/
int test_function(double a = 10) {
	globalvar.intarray = NULL;
	int value_pred = 100;
	if (value_pred > 10) {
		int def_var1 = 1;
		while (def_var1 >= 1 && (def_var1 <= 1 || value_pred > 10)) {
			for (int index = 0; index < 10; ++index) {
				//my_memcpy(NULL,NULL);
			}
		}
		int x = 32;
		int y = 100 / x;
		int z = x / 64;
	} else {
		int def_var2 = 1;
	}
	while (true) {
		for (int index = 0; index < 10; ++index) {
			//my_memcpy(NULL,NULL);
		}
		//my_memcpy(NULL,NULL);

	}
	//my_memcpy(NULL,NULL);
	return 100;
}
void do_calc(int test_name = 10) {
	int value = 0;
	for (int i = 0; i < 100; ++i) {
		value = value + i;
	}
	while (true) {
		value += 1;
		if (value = 100) {
			return;
		}
	}
	int x = 1001;
	value = x / 32;
	if (value == 0) {
		if (value == 1) {
			if (value == 2) {
				if (value == 3) {
				}
			}
		}
	}
	do_calc(100);
}
/*
int main(int argc, char** argvector) {
	int x, i;
	switch (x) {
	case 1:
{
	for (i = 0; i < 100; i++) {
		VOS_MemAlloc(10);
	}
		x++;
}
	default:
		;
	}
	return 0;
}
*/


int min(int i, int j)
{
	return i < j ? i : j;
}

int main(int argc, char **argv)
{
	int x, i;
	switch (x) {
	case 1:
	{
		for (i = 0; i < 100; i++) {
			VOS_MemAlloc(10);
		}
		x++;
	}
	break;
	default:
		break;
	}
	int sum = 0;

	for (int i = 0; i < argc; i++) {
		for (int j = 0; j < 10; j++) {
			sum += sum + i + j;
		}
	}

	int val = 1;
	switch (val) {
	case 1:
		val += 1;
		break;
	default:
		break;
	}

	int minval = 0;
	if (val <= 10) {
		minval = min(val, sum);
	}

	int count = 0;
	while (minval--) {
		count++;
	}
	return 0;
}
