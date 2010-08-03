/**
 * https://www.spoj.pl/problems/IKEYB/
 * Schier Michael
 **/
 
#include <stdio.h>

#define bool int
#define false 0
#define true 1
#define MAX_CHARS 90

// input
int numDigits, numLetters;
char digits[MAX_CHARS];
char letters[MAX_CHARS];
int cost[MAX_CHARS][MAX_CHARS]; // cost[1stLetter][numLettersAtDigit-1]


void readInput()
{
	int i;

	scanf("%d %d\n", &numDigits, &numLetters);
	
	for(i = 0; i < numDigits; i++)
		digits[i] = getchar();
		
	getchar(); // skip newline
	
	for(i = 0; i < numLetters; i++)
		letters[i] = getchar();
		
	for(i = 0; i < numLetters; i++)
		scanf("%d\n", &cost[i][0]);
}

int widths[MAX_CHARS][MAX_CHARS];
void printOutput(int row, int column)
{
	if (row > 0)
	{
		int i;
		printOutput(row-1, widths[row][column]);
		printf("%c: ", digits[row]);
		for (i=widths[row][column]+1; i<column+1; i++)
			printf("%c", letters[i]);
		printf("\n");
	}
	else
	{
		int i;
		printf("%c: ", digits[0]);
		for (i=0; i<column+1; i++)
			printf("%c", letters[i]);
		printf("\n");
	}
}


int tmp[MAX_CHARS][MAX_CHARS];
void optimize()
{
	int c, d, l, i, help;
	for(l = 0; l < numLetters; l++)
		for(c = 1; c <= numLetters - numDigits; c++)
			cost[l][c] = cost[l][c-1] + (c+1) * cost[l+c][0];
			
	for(c = 0; c <= numLetters - numDigits; c++)
		tmp[0][c] = cost[0][c];

	for(d = 1; d < numDigits; d++)
		for(c = 0; c <= numLetters - numDigits; c++)
		{
			tmp[d][c+d] = 1000000000;
			for(i = d - 1; i < d + c; i++)
			{
				help = tmp[d-1][i] + cost[i+1][d+c-i-1];
				if (tmp[d][d+c] > help)
				{
					tmp[d][d+c] = help;
					widths[d][d+c] = i;
				}
			}
		}
}
 
int main(int argc, char** args)
{
	int numTestCases;
	scanf("%d\n", &numTestCases);
	int i;
	for(i = 1; i <= numTestCases; i++)
	{
		readInput();
		optimize();
		
		printf("Keypad #%d:\n", i);
		printOutput(numDigits-1, numLetters-1);
		printf("\n");
	}

	return 0;
}

