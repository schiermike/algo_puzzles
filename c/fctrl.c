/**
 * https://www.spoj.pl/problems/FCTRL/
 * Schier Michael
 */
#include <stdio.h>

int main(int argc, char** args)
{
	int t;
	long long n;
	long long numZ;
	scanf("%i\n", &t);
	while(t-->0)
	{
		scanf("%lli\n", &n);
		numZ = 0;
		while(n > 0)
		{
			n /= 5;
			numZ += n;
		}
		printf("%lli\n", numZ);
	}
	return 0;
}

