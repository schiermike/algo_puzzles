/**
 * Test Generation Program for
 * https://www.spoj.pl/problems/AE4A/
 * Schier Michael
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define bool char
#define false 0
#define true !false

typedef struct
{
	int dim;
	bool c[1000][1000];
} Stamp;

Stamp s, k;

void print(Stamp* stmp)
{
	int i, j;
	for(i=0; i<stmp->dim; i++)
	{
		for(j=0; j<stmp->dim; j++)
			printf("%c", stmp->c[i][j] ? '1' : '0');
		printf("\n");
	}
}

int main()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	unsigned int seed = tv.tv_sec*1000000 + tv.tv_usec;
	srand(seed);

	s.dim = 3;
	k.dim = 5;
	
	int i, j;
	for(i=0; i<s.dim; i++)
		for(j=0; j<s.dim; j++)
			s.c[i][j] = rand() % 2;
			
	for(i=0; i<k.dim; i++)
		for(j=0; j<k.dim; j++)
			k.c[i][j] = false;
			
	int num = 1000;
	while(num-->0)
	{
		int hOff = rand() % (k.dim - s.dim);
		int wOff = rand() % (k.dim - s.dim);
		
		if(rand()%2) // apply small stamp
		{
			int h = rand() % (k.dim-2);
			int w = rand() % k.dim;
			
			k.c[h][w] ^= true;
			k.c[h+2][w] ^= true;
		}
			
		for(i=0; i<s.dim; i++)
			for(j=0; j<s.dim; j++)
				k.c[hOff + i][wOff + j] ^= s.c[i][j];
	}
	
	printf("1\n");
	printf("%d %d\n", s.dim, k.dim);
	printf("100\n");
	printf("000\n");
	printf("100\n");
	print(&s);
	print(&k);
	
	fprintf(stderr, "Successfully generated testcase with seed %u\n", seed);

	return 0;
}

