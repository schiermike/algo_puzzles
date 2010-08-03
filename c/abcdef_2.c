/**
 * https://www.spoj.pl/problems/ABCDEF/
 * Schier Michael
 *
 * Equation: ( a * b + c ) / d - e = f
 */

#include <stdio.h>

#define PRIME 2097152

typedef struct
{
	int number;
	int c1;
	int c2;
	int nextHash;
} HashItem;

int firsthash[PRIME];
int inhash;
HashItem hashItem[PRIME];

int findItem(int number)
{
	int h = (4000000000l+number)%PRIME;
	int e;
	
	for(e=firsthash[h]; e; e=hashItem[e].nextHash)
		if(hashItem[e].number == number)
			return e;
			
	++inhash;
	hashItem[inhash] = (HashItem){number, 0, 0, firsthash[h]};
	return firsthash[h]=inhash;
}


int main(int argc, char** args)
{
	int size;
	int s[101];

	scanf("%d\n", &size);
	int i, j, k;
	for(i=0; i<size; i++)
		scanf("%d\n", &s[i]);
		
	inhash = 0;
	int abc, def;
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
			for(k=0; k<size; k++)
			{
				abc = s[i] * s[j] + s[k];
				hashItem[findItem(abc)].c1++;
				if(!s[i])
					continue;
				def = s[i] * (s[j] + s[k]);
				hashItem[findItem(def)].c2++;
			}
			
	long long count = 0;
	for(i=1; i<=inhash; i++)
		count += hashItem[i].c1 * hashItem[i].c2;
		
	printf("%llu\n", count);
	
	return 0;
}

