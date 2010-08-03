
#include <stdlib.h>
#include <stdio.h>

#define boolean unsigned char
#define true 1
#define false 0
#define MAX_DEPTH 8

boolean guessConflict[MAX_DEPTH];
unsigned short sudokuGuess[MAX_DEPTH][256];
void solve(unsigned short* sudoku, int recursionLevel);


void setCell(unsigned short* sudoku, unsigned int row, unsigned int col, unsigned char bit)
{
	sudoku[row * 16 + col] = 1 << bit;
}

unsigned int indexOfOtherRowCells(unsigned int row, unsigned int col, unsigned int i)
{
	return 16 * row + (col + i) % 16;
}

unsigned int indexOfOtherColCells(unsigned int row, unsigned int col, unsigned int i)
{
	return 16 * ((row+i)%16) + col;
}

unsigned int indexOfOtherBoxCells(unsigned int row, unsigned int col, unsigned int i)
{
	return 16 * ((row/4)*4 + (row+i/4)%4) + (col/4)*4 + (col+i)%4;
}

void copySudoku(unsigned short* source, unsigned short* target)
{
	unsigned int i;
	for(i = 0; i < 256; i++)
		target[i] = source[i];
}

unsigned int countPossibilities(unsigned short value)
{
	unsigned short count = 0;
	unsigned short mask = 1;

	while(mask != 0)
	{
		if(value & mask)
			count++;
		mask <<= 1;
	}
	return count;
}

boolean isFixed(unsigned short value)
{
	return countPossibilities(value) == 1;
}

boolean isSolved(unsigned short* sudoku)
{
	unsigned int i;
	for(i = 0; i < 256; i++)
		if(!isFixed(sudoku[i]))
			return false;
	return true;
}

void printSudoku(unsigned short* sudoku)
{
	printf("==============================================================================================================================================================================================================================================================================================\n");
	unsigned int row, col;
	for(row = 0; row < 16; row++)
	{
		for(col = 0; col < 16; col++)
		{
			unsigned short indent = 18;
			unsigned short v = sudoku[row * 16 + col];
			unsigned char letter = 'A';
			while(v != 0)
			{
				if(v % 2)
				{
					printf("%c", letter);
					indent--;
				}
				v = v >> 1;
				letter++;
			}
			while(indent-->0)
				printf(" ");
		}
		printf("\n");
	}
	printf("==============================================================================================================================================================================================================================================================================================\n");
}

//------------------------------------------------------------------------------------------------

boolean solvedCellSimplifiesUnit(unsigned short* sudoku, int recursionLevel)
{
	boolean success = false;

	unsigned int row, col;
	for(row = 0; row < 16; row++)
		for(col = 0; col < 16; col++)
		{
			unsigned short candidates = sudoku[16 * row + col];
			if(candidates == 0)
			{
				guessConflict[recursionLevel] = true;
				return;
			}
			if(!isFixed(candidates))
				continue;

			unsigned int i;
			for(i = 1; i < 16; i++)
			{
				// remove from row
				unsigned int cell = indexOfOtherRowCells(row, col, i);
				success |= sudoku[cell] & candidates;
				sudoku[cell] &= ~candidates;
				
				// remove from col
				cell = indexOfOtherColCells(row, col, i);
				success |= sudoku[cell] & candidates;
				sudoku[cell] &= ~candidates;
				
				// remove from box
				cell = indexOfOtherBoxCells(row, col, i);
				success |= sudoku[cell] & candidates;
				sudoku[cell] &= ~candidates;
			}
		}
	if(success && recursionLevel < 0)
	{
		printf("SolvedCellSimplifiedUnit\n");
		printSudoku(sudoku);
	}
	return success;
}

boolean findSingles(unsigned short* sudoku, int recursionLevel)
{
	boolean success = false;

	unsigned int row, col;
	for(row = 0; row < 16; row++)
		for(col = 0; col < 16; col++)
		{
			if(isFixed(sudoku[16 * row + col]))
				continue;

			/**
			Collect all other cells from the current unit via OR, then invert the mask and AND it with the cell candidates
			**/
			unsigned int i;
			unsigned short mask = 0x0000;
			for(i = 1; i < 16; i++)
			{
				unsigned int cell = indexOfOtherRowCells(row, col, i);
				mask |= sudoku[cell];
			}
			if(sudoku[16 * row + col] & ~mask)
			{
				success = true;
				sudoku[16 * row + col] &= ~mask;
				continue;
			}

			mask = 0x0000;
			for(i = 1; i < 16; i++)
			{
				unsigned int cell = indexOfOtherColCells(row, col, i);
				mask |= sudoku[cell];
			}
			if(sudoku[16 * row + col] & ~mask)
			{
				success = true;
				sudoku[16 * row + col] &= ~mask;
				continue;
			}

			mask = 0x0000;
			for(i = 1; i < 16; i++)
			{
				unsigned int cell = indexOfOtherBoxCells(row, col, i);
				mask |= sudoku[cell];
			}
			if(sudoku[16 * row + col] & ~mask)
			{
				success = true;
				sudoku[16 * row + col] &= ~mask;
				continue;
			}
		}
	if(success && recursionLevel < 0)
	{
		printf("findSingles\n");
		printSudoku(sudoku);
	}
	return success;
}

boolean simplify(unsigned short* sudoku, int recursionLevel)
{
	boolean success = false;
	boolean roundSuccess = true;
	while(roundSuccess && !guessConflict[recursionLevel])
	{
		roundSuccess = false;
		roundSuccess |= solvedCellSimplifiesUnit(sudoku, recursionLevel);
		roundSuccess |= findSingles(sudoku, recursionLevel);
		

		success |= roundSuccess;
	}
	return success;
}

//------------------------------------------------------------------------------------------------

boolean guess(unsigned short* sudoku, int recursionLevel)
{
	if(recursionLevel >= MAX_DEPTH) // max
		return false;

	unsigned int cell, numPossibilities;
	for(cell = 0; cell < 256; cell++)
		for(numPossibilities = 2; numPossibilities < 16; numPossibilities++)
		{
			if(countPossibilities(sudoku[cell]) != numPossibilities)
				continue;

			unsigned int bit;
			for(bit = 0; bit < 16; bit++)
			{
				if(!(sudoku[cell] & (1 << bit)))
					continue;
				
				copySudoku(sudoku, sudokuGuess[recursionLevel]);
				// make a guess
				sudokuGuess[recursionLevel][cell] &= (1 << bit);
				guessConflict[recursionLevel] = false;

				solve(sudokuGuess[recursionLevel], recursionLevel);

				if(!guessConflict[recursionLevel])
					continue;

				// remove impossible guess from source sudoku
				sudoku[cell] &= ~(1 << bit);
				if(recursionLevel <= 0)
				{
					printf("Conflict at cell %d for value %c\n", cell, 'A' + bit);
				}
				return true;
			}

		}
		
	return false;
}

void solve(unsigned short* sudoku, int recursionLevel)
{
	do
	{
		boolean couldSimplify = simplify(sudoku, recursionLevel);
//		if(recursionLevel > 0 && !couldSimplify)
//			return;

		if(isSolved(sudoku))
			return;
	}
	while( guess(sudoku, recursionLevel+1) );
}

//------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	unsigned int numTests;
	scanf("%u\n", &numTests);

	unsigned int row, col;
	unsigned char line[17];
	unsigned short sudoku[256];
	while(numTests-->0)
	{
		for(row=0; row < 16; row++)
		{
			scanf("%s", line);
			for(col=0; col < 16; col++)
				switch(line[col])
				{
					case '-':
						sudoku[16 * row + col] = 0xffff;
						break;
					default:
						setCell(sudoku, row, col, line[col] - 'A');
				}
		}
		printSudoku(sudoku);
		solve(sudoku, -1);
	}

	return 0;
}

