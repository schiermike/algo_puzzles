/**
 * https://www.spoj.pl/problems/RESN04/
 * Schier Michael
 */

#include <stdio.h>

int main()
{
	int t;
	scanf("%d\n", &t);
	while(t-->0)
	{
		int sum = 0;
		int n, i, s;
		scanf("%d\n", &n);
		for(i = 1; i <= n; i++)
		{
			scanf("%d", &s);
			sum += s / i;
		}
		printf("%s\n", sum % 2 == 0 ? "BOB" : "ALICE");
	}	

	return 0;
}

