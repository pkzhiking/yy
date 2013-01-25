#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "test/test.h"
char ccc[100] = "ttttt";
void testlo()
{
testlocation();
}
void test1(int b)
{
int a;
if (b == 3)
{
return ;
}
a = 3 + 3;
if (a == 3)
a = 5;
}
void test2(int b)
{
int a;
if (b == 5)
{
a = 3 + 3;
if (a == 3)
a = 5;
}
}
void test3(int b)
{
if (b == 5)
{
int a;
a = 3 + 3;
if (a == 3)
a = 5;
}
}
int tt()
{
return -1;
}
void test_call()
{
 test1(1);
test2(1);
test3(1);
}
void test_switch()
{
int a, b, c, d;
if (a == 10)
{
a =3;
}
if (a==1)
{a =3;
}
if (3 == a)
{a =3;
}
if (5==a)
{a =3;
}
if (10 == a)
{
a =3;
}
else if (10==a)
{a =3;
}
else if  (3 == a)
{a =3;
}
else if (5==a)
{a =3;
}
if (10==a)
{a =3;
}
else if  (3 == a)
{a =3;
}
else if (5==a)
{a =3;
}
}
void test_switch1()
{
int a, b, c, d;
if (10 == tt())
{
a =3;
}
else if (10==a)
{a =3;
}
else if  (3 == a)
{a =3;
}
else if (5==a)
{a =3;
}
}

void test_strlen()
{
char tt[100] = "tttt";

for(int i = 0; i < strlen(tt); i++)
{
if (tt[i] == 'a')
tt[i] = 'b';
}
}
void test_strlen1()
{
char ttt[100] = "tttt";

for(int i = 0; strlen(ttt) < tt(); i++)
{
if (ccc[i] == 'a')
ccc[i] = 'b';
}
}
