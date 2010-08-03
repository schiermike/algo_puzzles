/**
 * https://www.spoj.pl/problems/MMIND/
 * Schier Michael
 */
#include <stdio.h>

#define bool int
#define true 1
#define false 0
#define MAX_MOVES 100
#define MAX_COLOR 101
#define MAX_POSITIONS 10

int numPins, numColors, numMoves; // the number of pins, colors and moves
int movePins[MAX_MOVES][MAX_POSITIONS]; // the color of the pin at the position within the move
int numPinsOk[MAX_MOVES]; // the number of correctly placed pins
int numPinColorsOk[MAX_MOVES]; // the number of misplaced pins with a matching color

int moveColors[MAX_MOVES][MAX_COLOR]; // the number of pins with the color within the move
int tmpColors[MAX_COLOR];
int guess[MAX_MOVES];


// increments the guess at the given position
bool incrementGuess(int index)
{
   int i;
   // reset post positions
	for(i = index + 1; i < numPins; i++)
		guess[i] = 1;

	// increment positions before index (carryover)
   for(i = index; i >= 0; i--)
   {
   	guess[i]++;
   	if(guess[i] <= numColors)
   		break;
   	guess[i] = 1;
   }

	// no higher guess possible
	if(i < 0)
		return false; 

	return true;
}

bool copyGuessedPin(int index, int moveIndex)
{
	if(guess[index] > movePins[moveIndex][index])
	{
		if(index > 0)
			return incrementGuess(index-1);
		return false;
	}
	
	guess[index] = movePins[moveIndex][index];
	int i;
	for(i = index+1; i < numPins; i++)
		guess[i] = 1;
	
	return true;
}

// return 0 means give up
// return 1 means restart checking loop
// return anything else means successful try
int checkMove(int index)
{
	int changeIndex = numPins;
	int copyIndex = numPins;
	int pinOk = numPinsOk[index];
	int pinNotOk = numPins - pinOk;
	
	int i;
	for(i = 0; i < numPins; i++)
	{
		if(movePins[index][i] == guess[i])
		{
			pinOk--;
			if(pinOk < 0)
			{
				changeIndex = i;
				break;
			}
		}
		else
		{
			pinNotOk--;
			if(pinNotOk < 0)
			{
				copyIndex = i;
				break;
			}
		}
	}
	
	for(i = 1; i <= numColors; i++)
		tmpColors[i] = moveColors[index][i];
		
	int pinColorOk = numPinsOk[index] + numPinColorsOk[index];
	int pinColorNotOk = numPins - pinColorOk;
	
	for(i = 0; i < numPins; i++)
	{
		tmpColors[guess[i]]--;
		if(tmpColors[guess[i]] >= 0)
			pinColorOk--;
		else
			pinColorNotOk--;
		
		if(pinColorOk < 0 || pinColorNotOk < 0)
		{
			if(changeIndex > i)
				changeIndex = i;
			break;
		}
	}
	
	if(changeIndex < copyIndex)
		return incrementGuess(changeIndex) ? 1 : 0;
		
	if(copyIndex < numPins)
		return copyGuessedPin(copyIndex, index) ? 1 : 0;
		
	return 2;
}

void solveProblem()
{
	int index, result;
	while(true)
	{
		for(index = 0; index < numMoves; index++)
		{
			result = checkMove(index);
			
			if(result <= 1)
				break;
		}
		
		if(result == 0)
		{
			printf("You are cheating!\n");
			return;
		}

		if(index != numMoves)
			continue;

		printf("%d", guess[0]);
		for(index=1; index < numPins; index++)
			printf(" %d", guess[index]);
		printf("\n");
		return;
	}
}

void readInput()
{
	scanf("%d %d %d\n", &numPins, &numColors, &numMoves);
	int i, j;
	for(i=0; i < numMoves; i++)
	{
		// reset colors
		for(j=1; j <= numColors; j++)
			moveColors[i][j] = 0;
	
		// read guess
		for(j=0; j < numPins; j++)
		{
			scanf("%d", &movePins[i][j]);
			moveColors[i][movePins[i][j]]++;
		}
	
		// read number of right / nearly right guesses
		scanf("%d %d\n", &numPinsOk[i], &numPinColorsOk[i]);
	}
	
	// smallest possible guess
	for(i=0; i<numPins; i++)
		guess[i] = 1;
}

int main(int argc, char** args)
{
	int numTests;
	scanf("%d\n", &numTests);
	while(numTests-->0)
	{
		readInput();
		solveProblem();
	}

	return 0;
}

