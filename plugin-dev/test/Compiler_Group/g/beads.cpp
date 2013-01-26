/*
ID: kiddlee1
PROG: beads
LANG: C++
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	ofstream fout("beads.out");
	ifstream fin("beads.in");

	int n;
	string beads;

	fin >> n >> beads;
	beads += beads;

	int max = 0;
	for (int i = 1; i < 2 * n - 1; ++i) {
		int r = 1, l = 1;
		char rc = 'w', lc = 'w';
		for (int j = i - 1; j - 1 >= 0; --j) {
			if (lc == 'w')
				lc = beads[j];

			if (lc == 'w')
				l++;
			else if (beads[j - 1] == 'w' || beads[j - 1] == lc)
				l++;
			else
				break;
		}

		for (int k = i; k + 1 <= 2 * n - 1; ++k) {
			if (rc == 'w')
				rc = beads[k];

			if (rc == 'w')
				r++;
			else if (beads[k + 1] == 'w' || beads[k + 1] == rc)
				r++;
			else
				break;
		}

		max = (l + r > max) ? (l + r) : max;
	}
	if (max > n)
		max = n;

	fout << max << endl;

	return 0;
}
