/**
 * https://www.spoj.pl/problems/GNYR09F/
 * Schier Michael
 */

#include <stdio.h>

#define MAX_PARAM 101

int resultF0[MAX_PARAM][MAX_PARAM];
int resultF1[MAX_PARAM][MAX_PARAM];
int fib[MAX_PARAM];

void reset()
{
	int i, j;
	for(i = 0; i < MAX_PARAM; i++)
	{
		for(j = 0; j < MAX_PARAM; j++)
			resultF0[i][j] = resultF1[i][j] = -1;
	}
	
	fib[0] = 0;
	fib[1] = 1;
	for(i = 2; i<MAX_PARAM; i++)
		fib[i] = fib[i-1] + fib[i-2];
	
}

int f1(int n, int k);
int f0(int n, int k)
{
	if(n==k || n==0 || n==k-1)
		return 0;
	if(k==0)
		return fib[n+1];

	if(resultF0[n][k] == -1)
		resultF0[n][k] = f0(n-1, k) + f1(n-1, k);
	return resultF0[n][k];
}

int f1(int n, int k)
{
	if(n==k || n==0)
		return 0;
	if(n==k-1)
		return 1;
	if(k==0)
		return fib[n];

	if(resultF1[n][k] == -1)
		resultF1[n][k] = f0(n-1, k) + f1(n-1, k-1);
	return resultF1[n][k];
}

void solve(int i, int n, int k)
{
	int result = f0(n, k) + f1(n, k);
	printf("%d %ld\n", i, result);
}

int main()
{
	int t;
	scanf("%d\n", &t);
	reset();
	while(t-->0)
	{
		int i, n, k;
		scanf("%d %d %d\n", &i, &n, &k);
		solve(i, n, k);
	}

	return 0;
}

