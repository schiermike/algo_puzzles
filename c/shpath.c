/**
 * https://www.spoj.pl/problems/SHPATH/
 * Schier Michael
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define null NULL
#define bool int
#define false 0
#define true !false
#define INFINITY 1000000000

typedef struct __neighbor
{
	int cityId;
	int cost;
	struct __neighbor* next;
} CityNeighbor;

int numCities;
CityNeighbor* neighbor[10000];
char cityMap[10000][11];

int pathCost[10000];
int cityQueue[10000];
int lastQueueIndex;

void findPath(int city1, int city2)
{
	int i;
	for(i = 0; i < numCities; i++)
		pathCost[i] = INFINITY;

	pathCost[city1] = 0;
	cityQueue[0] = city1;
	lastQueueIndex = 0;
	while(lastQueueIndex >= 0)
	{
		int minI = 0;
		for(i = 1; i <= lastQueueIndex; i++)
		{
			if(pathCost[cityQueue[i]] < pathCost[cityQueue[minI]] )
				minI = i;
		}
		int city = cityQueue[minI];
		cityQueue[minI] = cityQueue[lastQueueIndex--]; // fill gap
		
		if(city == city2)
			break;
			
		CityNeighbor* n = neighbor[city];
		while(n)
		{
			int sumCost = pathCost[city] + n->cost;
			if(pathCost[n->cityId] > sumCost)
			{
				if(pathCost[n->cityId] == INFINITY)
					cityQueue[++lastQueueIndex] = n->cityId; // new reachable city, enqueue
				pathCost[n->cityId] = sumCost;
			}
			n = n->next;
		}
	}
	
	printf("%d\n", pathCost[city2] < INFINITY ? pathCost[city2] : 0);
	
}

void testCase()
{
	memset(cityMap, 0, 10000*11);

	scanf("%d\n", &numCities);
	int id;
	char name[11];
	for(id = 0; id < numCities; id++)
	{
		scanf("%s\n", cityMap[id]);
		int p;
		scanf("%d\n", &p);
		
		neighbor[id] = null;
		CityNeighbor* n;
		while(p-->0)
		{
			if(neighbor[id] == null)
			{
				neighbor[id] = (CityNeighbor*)malloc(sizeof(CityNeighbor));
				n = neighbor[id];
			}
			else
			{
				n->next = (CityNeighbor*)malloc(sizeof(CityNeighbor));
				n = n->next;
			}
			
			scanf("%d %d\n", &n->cityId, &n->cost);
			n->cityId--;
			n->next = null;
		}
	}
	int r;
	scanf("%d\n", &r);
	while(r-->0)
	{
		int id1, id2;
		
		scanf("%s ", name);
		for(id1 = 0; id1 < 10000; id1++)
			if(strncmp(cityMap[id1], name, 10) == 0)
				break;
				
		scanf("%s ", name);
		for(id2 = 0; id2 < 10000; id2++)
			if(strncmp(cityMap[id2], name, 10) == 0)
				break;
				
		findPath(id1, id2);
	}
}

int main(int argc, char** args)
{
	int t;
	scanf("%d\n", &t);
	while(t-->0)
		testCase();
		
	return 0;
}

