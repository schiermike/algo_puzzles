/**
 * https://www.spoj.pl/problems/LATTICE/
 * Schier Michael
 */

#include <stdio.h>
#include <math.h>
#define MAXN 5000

// calculate the number of connections of length sqrt(w^2+h^2) in a n^2 grid
long numConn(int n, int w, int h)
{
	long result = (n-w) + (n-h) - 1;
	return w==h || w==0 || h==0 ? result*2 : result*4;
}

/**
 * extend lattice
 * example: n=5, new is #
 *   #####
 *   #****
 *   #*---
 *   #*-;;
 *   #*-;.
 */
double calc(int n, double result)
{
	int w,h;
	for(w=0; w<n; w++)
		for(h=w; h<n; h++)
		{
			if(h==0)
				continue;
#ifdef DEBUG
			printf("-> %ld x sqrt(%d)\n", numConn(n, w, h), w*w + h*h);
#endif
			result += numConn(n, w, h) * sqrt(w*w + h*h);
		}
	
	return result;
}

int main()
{
	long long int i;
	double result = 0.0;
	for(i=1; i<=MAXN; i++)
	{
		result = calc(i, result);
		printf("%.2lf\n", 2*result / (i*i*i*i));
		fprintf(stderr, "%d\n", i);
	}
	return 0;
}

