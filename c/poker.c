/**
 * https://www.spoj.pl/problems/POKER/
 * Schier Michael
 */

#include <stdio.h>
#define bool char
#define false 0
#define true !false

char hand[15];

bool checkStraight()
{
	return (hand[0]+1 == hand[3]) && (hand[3]+1 == hand[6]) && (hand[6]+1 == hand[9]) && ( (hand[9]+1 == hand[12]) || (hand[12] == 14 && hand[0] == 2) );
}

bool checkFlush()
{
	return (hand[1] == hand[4]) && (hand[1] == hand[7]) && (hand[1] == hand[10]) && (hand[1] == hand[13]);
}

char* analyse()
{
	bool isStraight = checkStraight();
	bool isFlush = checkFlush();
	
	if(isStraight)
	{
		if(isFlush)
			return hand[0] == 10 ? "royal flush" : "straight flush";
		return "straight";
	}
	if(isFlush)
		return "flush";
		
	int sign[5];
	int freq[5];
	int i;
	for(i=0; i<5; i++)
		sign[i] = freq[i] = 0;
		
	for(i=0; i<5; i++)
	{
		int j = 0;
		while(sign[j]!=0 && sign[j]!= hand[i*3])
			j++;
		sign[j] = hand[i*3];
		freq[j]++;
	}
		
	if(freq[0] == 4 || freq[1] == 4)
		return "four of a kind";
		
	if(freq[0] == 3 || freq[1] == 3 || freq[2] == 3)
	{
		if(freq[0] == 2 || freq[1] == 2 || freq[2] == 2)
			return "full house";
		return "three of a kind";
	}
	
	int pairCount=0;
	for(i=0; i<4; i++)
		if(freq[i] == 2)
			pairCount++;
			
	if(pairCount>0)
		return pairCount==2 ? "two pairs" : "pair";
	
	
	
	return "high card";
}

// sort cards by their numbers in ascending order
void sort()
{
	int i, j;
	for(i=0; i<4; i++)
		for(j=i+1; j<5; j++)
			if(hand[i*3] > hand[j*3])
			{
				char tmp = hand[i*3];
				hand[i*3] = hand[j*3];
				hand[j*3] = tmp;
				tmp = hand[i*3+1];
				hand[i*3+1] = hand[j*3+1];
				hand[j*3+1] = tmp;
			}
}

// convert card numbers to interval 2-14
void convert()
{
	int i;
	for(i=0; i<5; i++)
	{
		switch(hand[i*3])
		{
			case 'A':
				hand[i*3] = 14;
				break;
			case 'K':
				hand[i*3] = 13;
				break;
			case 'Q':
				hand[i*3] = 12;
				break;
			case 'J':
				hand[i*3] = 11;			
				break;
			case 'T':
				hand[i*3] = 10;
				break;
			default:
				hand[i*3] -= 48;
				break;
		}
	}
}
 
int main(int argc, char** args)
{
	int numTests;
	scanf("%d\n", &numTests);
	while(numTests-->0)
	{
		scanf("%s", &hand[0]);
		scanf("%s", &hand[3]);
		scanf("%s", &hand[6]);
		scanf("%s", &hand[9]);
		scanf("%s\n", &hand[12]);
		convert();
		sort();
		printf("%s\n", analyse());
	}

	return 0;
}

