/**
 * https://www.spoj.pl/problems/RPS/
 * Schier Michael
 */

#include <stdio.h>
#define bool char
#define false 0
#define true !false
#define MAXWINS 30

int m, n;
int win[MAXWINS];

int solve()
{
	int i = 0;
	for(i=0; i<MAXWINS; i++)
		win[i] = 0;
	win[0] = n;
	
	int rounds = 0;
	while(win[m] == 0)
	{
		rounds++;
		int thisRoundLoosers = 0;
		for(i=m-1; i>=0; i--)
		{
			if(win[i] <= 1)
				continue;
			
			win[i+1] += win[i] / 2;
			thisRoundLoosers += win[i] / 2;
			win[i] %= 2;
		}
		win[0] += thisRoundLoosers;
		
#ifdef DEBUG
		printf("Round %2d: ", rounds);
		for(i=0; i<m; i++)
			printf("%2d ", win[i]);
		printf("\n");
#endif
	}
	
	return rounds;
}

int main()
{
	int i = 1;
	while(true)
	{
		scanf("%d %d\n", &n, &m);
		if(m==0 && n==0)
			break;
		printf("Case %d: %d\n", i++, solve());
		
	}
	
	return 0;
}

