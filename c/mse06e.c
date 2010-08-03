/**
 * https://www.spoj.pl/problems/MSE06E/
 * Schier Michael
 */

#include <stdio.h>
#include <math.h>

void solveForHuge(double b)
{
	int logB = log(b)/log(2);
	int exps[10];
	int i;
	for(i=0; i<10; i++)
		exps[i] = 0;
	i = 0;
	while(logB > 5)
	{
		exps[i++] = 2;
		logB /= 2;
	}
	if(logB == 5)
	{
		exps[i++] = 5;
	}
	else if(logB == 4)
	{
		exps[i++] = 2;	
		exps[i++] = 2;	
	}
	else if(logB == 3)
	{
		exps[i-1] = 3;
		exps[i++] = 2;
	}
	// else should not happen
	
	printf("2");
	for(i=0; i<10; i++)
	{
		if(exps[i] == 0)
			break;
		printf("^%d", exps[i]);
	}
	printf("\n");				
}

void solve(double b)
{
	if(b>=64)
		solveForHuge(b);
	else if(b == 49)
		printf("7^2\n");
	else if(b == 36)
		printf("6^2\n");
	else if(b ==25 )
		printf("5^2\n");
	else if(b == 16)
		printf("2^2^2\n");
	else if(b==9)
		printf("3^2\n");
	else if(b == 8)
		printf("2^3\n");
	else if(b==4)
		printf("2^2\n");
	else
		printf("%.0f\n", b);
		
}

int main()
{
	double balance;
	while(scanf("%lf\n", &balance) == 1)
	{
		solve(balance);
	}
	return 0;
}

