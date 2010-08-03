
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

unsigned int* divisors;
unsigned int numDivisors;

void createDivisors(unsigned int maxCandidate) 
{
	divisors = (unsigned int*)malloc(sizeof(unsigned int)*maxCandidate/2);
	divisors[0] = 2;
	divisors[1] = 3;
	divisors[2] = 5;
	divisors[3] = 7;
	divisors[4] = 11;
	
	numDivisors = 5;
	
	unsigned int candidate;
	for(candidate = 13; candidate <= maxCandidate; candidate++)
	{
		unsigned int sqrtCandidate = (unsigned int)sqrt(candidate);
		int isPrime = 1;
		unsigned int index;
		for(index = 0; index < numDivisors; index++)
		{
			unsigned int prime = divisors[index];
			if(prime > sqrtCandidate)
				break;
			if(candidate % prime == 0)
			{
				isPrime = 0;
				break;
			}
		}
		if(isPrime)
			divisors[numDivisors++] = candidate;
	}
}

void findPrimes(unsigned int min, unsigned int max)
{
	unsigned int sqrtCandidate;
	int isPrime;
	if(min <= 2)
	{
		printf("2\n");
		min = 3;
	}
	
if(min % 2 == 0)
		min++;
		
	unsigned int candidate;
	for(candidate = min; candidate <= max; candidate+=2)
	{
		if(candidate == 1)
			continue;
		if(candidate > 20)
		{
			if(candidate % 19 == 0 || candidate % 17 == 0 || candidate % 13 == 0 || candidate % 11 == 0 || candidate % 7 == 0 || candidate % 5 == 0 || candidate % 3 == 0)
				continue;
		}
			
		sqrtCandidate = (unsigned int)sqrt(candidate);
		isPrime = 1;
		int index;
		for(index = 0; index < numDivisors; index++)
		{
			unsigned int prime = divisors[index];
			if(prime > sqrtCandidate)
				break;
			if(candidate % prime == 0)
			{
				isPrime = 0;
				break;
			}
		}
		if(isPrime)
			printf("%u\n", candidate);
		}
}

int main(int argc, char** argv)
{
	unsigned int numTests;
	scanf("%u\n", &numTests);

	unsigned int* min = (unsigned int*)malloc(sizeof(unsigned int) * numTests);
	unsigned int* max = (unsigned int*)malloc(sizeof(unsigned int) * numTests);
	unsigned int i;
	unsigned int maxmax = 0;
	for(i = 0; i < numTests; i++)
	{
		scanf("%u %u\n", &min[i], &max[i]);
		if(maxmax < max[i])
			maxmax = max[i];
	}

	unsigned int sqrtMax = (unsigned int)sqrt(maxmax);
	createDivisors(sqrtMax);

	for(i = 0; i < numTests; i++)
	{
		findPrimes(min[i], max[i]);
		if(i < numTests-1) 
			printf("\n");
	}


	return 0;
}

