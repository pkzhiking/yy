int mymax(int a, int b)
{
	return a > b ? a : b;
}

int main()
{
	// 3 types of loops, each with one or many expression including another loops or not.
	int sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += i;
		if (sum % 2 == 0)
			sum += 1;
		for (int j = 0; j < 5; j++) {
			if (j != i)
				sum += j;
            else {
                sum -=j;
            }
		}
	}
	while (sum < 3000) {
		for (int i = 0; i < 5; i++) {
			sum += i;
		}
	}
	do {
		sum += 155;
	} while (sum < 400);
	return 0;
}
