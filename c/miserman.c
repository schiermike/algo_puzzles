#include <stdio.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true (!false)

#define MAX(a,b) ((a)<(b)?(b):(a))
#define MIN(a,b) ((a)<(b)?(a):(b))

int numCities;
int numBusses;

int costs[100][100];
int minBusIdx[100];
int busIdx[100];
int minCost = 100*100;

void search(int city, int cost)
{
	int bus;
	int minBus = city == 0 ? 0 : MAX(0, busIdx[city-1]-1);
	int maxBus = city == 0 ? numBusses-1 : MIN(busIdx[city-1]+1, numBusses-1);
	for(bus = minBus; bus <= maxBus; bus++)
	{
		busIdx[city] = bus;
		int newCost = cost + costs[city][bus];
		// already too expensive
		if(newCost > minCost)
			continue;
		if(city + 1 < numCities)
			search(city+1, newCost);
		else if(newCost < minCost)// we reached the last city and have found a cheaper solution
		{
			minCost = newCost;
			int j;
			for(j = 0; j < numCities; j++)
				minBusIdx[j] = busIdx[j];
		}
	}
}

int main(int argc, char** args)
{
	scanf("%d %d\n", &numCities, &numBusses);

	int i, j;
	for(i = 0; i < numCities; i++)
	{
		for(j = 0; j < numBusses; j++)
		{
			scanf("%d", &costs[i][j]);
		}
	}

	search(0, 0);

	printf("%d\n", minCost);
//	for(i = 0; i < numCities; i++)
//		printf("%d ", minBusIdx[i]);
//	printf("\n");

	return 0;
}

