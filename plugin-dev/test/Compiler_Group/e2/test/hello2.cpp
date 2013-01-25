#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct cstruct
{
    char p;
    static const int x = 10;
    double y;
    int *intarray;
    double *doublearray;
    int z;
};
struct simplepod
{
    int x;
    int y;
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
cstruct globalvar;
int test_stack_simple()
{
    int intVar1;
    int intVar2;
}
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
}
int test_stack()
{
    int x;
    if(x>0)
    {
        int var1[10];
    }
    else
    {
        int var2[20];
    }
    return 0;
}
int func1()
{
    int intVar1;
    int intVar2;
    if(intVar1>0)
    {
        int arrVar1[10];
    }
    else
    {
        if(intVar2>10)
        {
            double doubleArrVar1[20]; 
        }
        else
        {
            float floatArrVar2[33]; 
        }
        int var2[20];
    }
    memset(&intVar1,100,10);
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
    for(int i=0;i<100;i)
    {
        ptr2[i] = 0x0;
        intVar = 1001;
    }
    memset(ptr2,0,100);
    memset(&globalvar.y,5,3);
    int p;
    p = 0x0;
    char n;
    strcpy(NULL,NULL);
    int value_pred = 100;
    memset(&doubleVar,0,4);
    if(value_pred>10)
    {
        int def_var1 = 1;
        while(def_var1>=1 && (def_var1<=1 || value_pred>10))
            {
                strncpy(NULL,NULL,100);
                for(int index = 0;index<10;++index)
                {
                }
            }
    int x = 32;
    int y = 100/x;
    int n = 100/x;
    int z = x/64;
    x = y*128;
    z = y*133;
    }
    else
    {
        strcpy(NULL,NULL);
        int def_var2 = 1;
    }
    memset(structPtr,10,5);
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
    //my_memcpy(NULL,NULL);
    return 100;     

}
int t1()
{

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
 //                       strncpy(NULL,NULL,j);
                    }
                }
                //my_memcpy(NULL,NULL);

    }
    //my_memcpy(NULL,NULL);
    return 100;     
}
int test_function(double a = 10)
{
    globalvar.intarray = NULL;
    int value_pred = 100;
    if(value_pred>10)
    {
        int def_var1 = 1;
        while(def_var1>=1 && (def_var1<=1 || value_pred>10))
            {
                for(int index = 0;index<10;++index)
                {
                    //my_memcpy(NULL,NULL);
                }
            }
    int x = 32;
    int y = 100/x;
    int z = x/64;
    }
    else
    {
        int def_var2 = 1;
    }
    while(true)
    {
                for(int index = 0;index<10;++index)
                {
                    //my_memcpy(NULL,NULL);
                }
                //my_memcpy(NULL,NULL);

    }
    //my_memcpy(NULL,NULL);
    return 100;
}
void do_calc(int test_name=10)
{
    int value = 0;
    for(int i=0;i<100;++i)
    {
        value = value + i;
    }
    while(true){
      value +=1 ; 
      if(value=100)
      {
          return;
      }
    }
    int x = 1001;
    value = x/32;
    if (value==0){
        if (value==1)
        {
            if(value == 2)
            {
                if(value ==3){}
            }
        }
    }
    do_calc(100);
}
int main(int argc,char** argvector)
{
    int x;
    switch(x)
    {
        case 1:
            x++;
        default:
            ;
    }
    return 0;
}

