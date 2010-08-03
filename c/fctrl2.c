/**
 * https://www.spoj.pl/problems/IKEYB/
 * Schier Michael
 **/
 
#include <stdio.h>

#define bool int
#define false 0
#define true 1

#define MAX_N 101
#define MAX_LEN 20
#define MAXI 1000000000
#define NUM_DIGITS 9
int fac[MAX_N][MAX_LEN]; 

// -1 indicates the start of the number
void precompute()
{
	fac[0][MAX_LEN-1] = 0; // result of 0!
	fac[0][MAX_LEN-2] = -1;
	fac[1][MAX_LEN-1] = 1; // result of 1!
	fac[1][MAX_LEN-2] = -1;

	int n, l;
	long long p;	
	for(n = 2; n < MAX_N; n++)
	{
		for(l = MAX_LEN-1; l >= 0; l--)
		{
			p = fac[n-1][l];
			if(p == -1) // end of previous digit
			{
				if(fac[n][l] == 0)
					fac[n][l] = -1;
				else
					fac[n][l-1] = -1;
				break;
			}
			
			p = p * n;
			p = p + fac[n][l];
			
			fac[n][l] = p % MAXI;
			fac[n][l-1] = p / MAXI;
		} 
	}
}

void factorial(n)
{
	if(n == 0)
	{
		printf("0\n");
		return;
	}
	if(n == 1)
	{
		printf("1\n");
		return;
	}
	
	int l;
	bool started = false;
	for(l = 0; l < MAX_LEN; l++)
	{
		if(fac[n][l] == -1)
			continue;
			
		if(!started)
		{
			if(fac[n][l] == 0)
				continue;
			printf("%d", fac[n][l]);
			started = true;
			continue;
		}
		
		// print leading zeros
		int t = fac[n][l];
		int numD = NUM_DIGITS;
		while(t != 0)
		{
			t /= 10;
			numD--;
		}
		while(numD-->0)
			printf("0");
			
		if(fac[n][l] != 0)
			printf("%d", fac[n][l]);
	}
	printf("\n");
}
 
int main(int argc, char** args)
{
	precompute();

	int numTestCases;
	scanf("%d\n", &numTestCases);
	int n;
	while(numTestCases-->0)
	{
		scanf("%d\n", &n);
		factorial(n);
	}

	return 0;
}

