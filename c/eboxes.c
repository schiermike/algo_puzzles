/**
 * https://www.spoj.pl/problems/EBOXES/
 * Schier Michael
 */

#include <stdio.h>

int main()
{
	int numT, n, k, t, f;
	scanf("%d\n", &numT);
	while(numT-->0)
	{
		scanf("%d %d %d %d\n", &n, &k, &t, &f);
		printf("%d\n", f + (f-n)/(k-1));
	}

	return 0;
}

