/* sin example */
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int myadd(int num1, int num2, int num3, int num4)
{
	return num1 + num2 + num3 + num4;
}

int main()
{
	double param, result;
	param = 30.0;
	result = sin((float)(param * PI / 180));

	int res = myadd(1, 2, 3, 4);

	if (res >= param && result < res) {
		res += 1;
	}

	return 0;

}
