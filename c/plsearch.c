/**
 * https://www.spoj.pl/problems/PLSEARCH/
 * Schier Michael
 */

#include <stdio.h>
#include <stdlib.h>
#define bool char
#define false 0
#define true !false

typedef struct
{
	int segLen[10];
	char segDir[10];
	int num;
} PolyLine;

PolyLine template;
PolyLine sample;

void scanPolyLine(PolyLine* polyLine)
{
	int n;
	scanf("%d\n", &n);
	
	int x, xOld, y, yOld, dir, dirOld;
	polyLine->num = 0;
	int i;
	for(i=0; i<n; i++)
	{
		scanf("%d %d\n", &x, &y);
		
		if(i>=1)
		{
			// calculate direction: 0=LEFT, 1=UP, 2=RIGHT, 3=DOWN
			if(yOld == y)
				dir = xOld < x ? 2 : 0;
			else
				dir = yOld < y ? 1 : 3;
				
			// calculate length
			polyLine->segLen[polyLine->num] = x == xOld ? abs(y-yOld) : abs(x-xOld);
			polyLine->num++;
		}
		
		if(i>=2)
		{
			// calculate movement:
			polyLine->segDir[polyLine->num-2] = (dirOld+1) % 4 == dir ? 'R' : 'L';
		}
		
		xOld = x;
		yOld = y;
		dirOld = dir;
	}
}

bool polyLineCompare()
{
	if(template.num != sample.num)
		return false;
		
	bool frontCheck = true;
	bool backCheck = true;
	int i;
	for(i=0; i<template.num; i++)
	{
		frontCheck &= (template.segLen[i] == sample.segLen[i]);
		backCheck &= (template.segLen[i] == sample.segLen[sample.num-1-i]);
	}
	for(i=0; i<template.num-1; i++)
	{
		frontCheck &= (template.segDir[i] == sample.segDir[i]);
		backCheck &= (template.segDir[i] != sample.segDir[sample.num-2-i]);
	}
	
	return frontCheck || backCheck;
}

int main()
{
	int n;
	while(scanf("%d\n", &n) == 1 && n != 0)
	{
		scanPolyLine(&template);
	
		int i;
		for(i=1; i<=n; i++)
		{
			scanPolyLine(&sample);
			if(polyLineCompare())
				printf("%d\n", i);
		}
		printf("+++++\n");
	}
	
	return 0;
}

