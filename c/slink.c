/**
 * https://www.spoj.pl/problems/SLINK/
 * Schier Michael
 */

#include <stdio.h>
#define bool char
#define false 0
#define true !false

#define PENDING 0
#define BLACK 1
#define WHITE 2

#define LEFT 0
#define TOP 1
#define RIGHT 2
#define BOTTOM 3

typedef struct
{
	int numEdges;
	int edge[4];
} Cell;

int rows, cols;
Cell cell[20][20];
bool isRoundSuccessful;

void print(int pv)
{
	int i;
	for(i=0; i<4*(cols+1)+1; i++)
		printf("#");
	printf("\n#");
	for(i=0; i<4*(cols+1)-1; i++)
		printf(" ");
	printf("#\n");
	
	int r,c;
	for(r=0; r<rows; r++)
	{
		printf("# ");
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].edge[TOP] == pv || c>0 && cell[r][c-1].edge[TOP] == pv)
			{
				if(cell[r][c].edge[LEFT] == pv || r>0 && cell[r-1][c].edge[LEFT] == pv)
					printf("+");
				else
					printf("-");
			}
			else if(cell[r][c].edge[LEFT] == pv && r>0 && cell[r-1][c].edge[LEFT] == pv)
				printf("|");
			else
				printf(" ");
			if(cell[r][c].edge[TOP] == pv)
				printf("---");
			else
				printf("   ");
		}
		if(cell[r][cols-1].edge[TOP] == pv)
		{
			if(cell[r][cols-1].edge[RIGHT] == pv || r>0 && cell[r-1][cols-1].edge[RIGHT] == pv)
				printf("+");
			else
				printf("-");
		}
		else if(cell[r][cols-1].edge[RIGHT] == pv && r>0 && cell[r-1][cols-1].edge[RIGHT] == pv)
			printf("|");
		else 
			printf(" ");
		
		printf(" #\n");

		printf("#");
		for(c=0; c<cols; c++)
		{
			printf(" ");
			if(cell[r][c].edge[LEFT] == pv)
				printf("|");
			else
				printf(" ");
			printf(" %d", cell[r][c].numEdges);
		}
		printf(" ");
		if(cell[r][cols-1].edge[RIGHT] == pv)
				printf("|");
			else
				printf(" ");
		printf(" #\n");
	}
	
	printf("# ");
	for(c=0; c<cols; c++)
	{
		if(cell[rows-1][c].edge[BOTTOM] == pv || c>0 && cell[rows-1][c-1].edge[BOTTOM] == pv)
		{
			if(cell[rows-1][c].edge[LEFT] == pv )
				printf("+");
			else
				printf("-");
		}
		else
			printf(" ");
		if(cell[rows-1][c].edge[BOTTOM] == pv)
			printf("---");
		else
			printf("   ");
	}
	if(cell[rows-1][cols-1].edge[BOTTOM] == pv)
	{
		if(cell[rows-1][cols-1].edge[RIGHT] == pv)
			printf("+");
		else
			printf("-");
	}
	else 
		printf(" ");
	printf(" #\n");	
	
	printf("#");
	for(i=0; i<4*(cols+1)-1; i++)
		printf(" ");
	printf("#\n");
	for(i=0; i<4*(cols+1)+1; i++)
		printf("#");
	printf("\n"); 
}

inline bool isWhite(int r, int c, int o)
{
	if(r < 0 || r >= rows)
		return true;
	if(c < 0 || c >= cols)
		return true;
	return cell[r][c].edge[o] == WHITE;
}

inline bool isBlack(int r, int c, int o)
{
	if(isWhite(r, c, o))
		return false;
		
	return cell[r][c].edge[o] == BLACK;
}

inline bool isPending(int r, int c, int o)
{
	if(isWhite(r, c, o))
		return false;
		
	return cell[r][c].edge[o] == PENDING;
}

inline void setEdgeState(int r, int c, int o, int v)
{
	if(r<0 || r >= rows)
		return;
	if(c<0 || c >= cols)
		return;

	if(isPending(r, c, o))
	{
		cell[r][c].edge[o] = v;
		isRoundSuccessful = true;
	}
	switch(o)
	{
		case LEFT:
			if(c>0 && isPending(r, c-1, RIGHT))
			{
				cell[r][c-1].edge[RIGHT] = v;
				isRoundSuccessful = true;
			}
			break;
		case RIGHT:
			if(c<cols-1 && isPending(r, c+1, LEFT))
			{
				cell[r][c+1].edge[LEFT] = v;
				isRoundSuccessful = true;
			}
			break;
		case TOP:
			if(r>0 && isPending(r-1, c, BOTTOM))
			{
				cell[r-1][c].edge[BOTTOM] = v;
				isRoundSuccessful = true;
			}
			break;
		case BOTTOM:
			if(r<rows-1 && isPending(r+1, c, TOP))
			{
				cell[r+1][c].edge[TOP] = v;
				isRoundSuccessful = true;
			}
			break;
	}
}

void ruleZeroFour() // RULE 1
{
	int r, c, o;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges == 0)
				for(o=0; o<4; o++)
					setEdgeState(r, c, o, WHITE);
			if(cell[r][c].numEdges == 4)
				for(o=0; o<4; o++)
					setEdgeState(r, c, o, BLACK);
		}
}

void ruleCornerElements()
{
	if(cell[0][0].numEdges == 1)
	{
		setEdgeState(0, 0, LEFT, WHITE);
		setEdgeState(0, 0, TOP, WHITE);
	}
	if(cell[0][0].numEdges == 3)
	{
		setEdgeState(0, 0, LEFT, BLACK);
		setEdgeState(0, 0, TOP, BLACK);
	}
	
	if(cell[0][cols-1].numEdges == 1)
	{
		setEdgeState(0, cols-1, RIGHT, WHITE);
		setEdgeState(0, cols-1, TOP, WHITE);
	}
	if(cell[0][cols-1].numEdges == 3)
	{
		setEdgeState(0, cols-1, RIGHT, BLACK);
		setEdgeState(0, cols-1, TOP, BLACK);
	}
	
	if(cell[rows-1][0].numEdges == 1)
	{
		setEdgeState(rows-1, 0, LEFT, WHITE);
		setEdgeState(rows-1, 0, BOTTOM, WHITE);
	}
	if(cell[rows-1][0].numEdges == 3)
	{
		setEdgeState(rows-1, 0, LEFT, BLACK);
		setEdgeState(rows-1, 0, BOTTOM, BLACK);
	}
	
	if(cell[rows-1][cols-1].numEdges == 1)
	{
		setEdgeState(rows-1, cols-1, RIGHT, WHITE);
		setEdgeState(rows-1, cols-1, BOTTOM, WHITE);
	}
	if(cell[rows-1][cols-1].numEdges == 3)
	{
		setEdgeState(rows-1, cols-1, RIGHT, BLACK);
		setEdgeState(rows-1, cols-1, BOTTOM, BLACK);
	}
}

void ruleNeighborThree()
{
	int r, c;
	for(r=0; r<rows-1; r++)
		for(c=0; c<cols-1; c++)
		{
			if(cell[r][c].numEdges != 3)
				continue;
			if(cell[r][c+1].numEdges == 3) // RULE 6
			{
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r, c, RIGHT, BLACK);
				setEdgeState(r, c+1, RIGHT, BLACK);
			}
			if(cell[r+1][c].numEdges == 3)  // RULE 6
			{
				setEdgeState(r, c, TOP, BLACK);
				setEdgeState(r, c, BOTTOM, BLACK);
				setEdgeState(r+1, c, BOTTOM, BLACK);
			}
			if(cell[r+1][c+1].numEdges == 3)  // RULE 7
			{
				setEdgeState(r, c, TOP, BLACK);
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r+1, c+1, BOTTOM, BLACK);
				setEdgeState(r+1, c+1, RIGHT, BLACK);
			}
		}
}

void ruleFixRemaining()
{
	int r, c, o;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			int numPending = 0;
			int numBlack = 0;
			int numWhite = 0;
			for(o=0; o<4; o++)
				switch(cell[r][c].edge[o])
				{
					case PENDING:
						numPending++;
						break;
					case BLACK:
						numBlack++;
						break;
					case WHITE:
						numWhite++;
						break;
				}
			if(numPending == 0)
				continue;
			if(numBlack == cell[r][c].numEdges) // replace pending with white
			{
				for(o=0; o<4; o++)
					setEdgeState(r, c, o, WHITE); // RULE 4+5
			}
			else if(numPending == cell[r][c].numEdges - numBlack) // replace pending with black
			{
				for(o=0; o<4; o++)
					setEdgeState(r, c, o, BLACK); // RULE 2+3
			}
		}
}

void ruleRemoveWrongTurns()
{
	int r, c, o;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
			for(o=0; o<4; o++)
			{
				if(!isBlack(r, c, o))
					continue;
				switch(o)
				{
					case LEFT:
						if(isBlack(r, c-1, TOP)) // ^<
						{
							setEdgeState(r-1, c, LEFT, WHITE); // ^^
							setEdgeState(r, c, TOP, WHITE); // ^>
						}
						if(isBlack(r-1, c, LEFT)) // ^^
						{
							setEdgeState(r, c-1, TOP, WHITE); // ^<
							setEdgeState(r, c, TOP, WHITE);	 // ^>
						}
						if(isBlack(r, c, TOP)) // ^>
						{
							setEdgeState(r-1, c, LEFT, WHITE); // ^^
							setEdgeState(r, c, TOP, WHITE); // ^>
						}
						break;
					case TOP:
						if(isBlack(r-1, c, RIGHT)) // >^
						{
							setEdgeState(r, c+1, TOP, WHITE); // >>
							setEdgeState(r, c, RIGHT, WHITE); // >.
						}
						if(isBlack(r, c+1, TOP)) // >>
						{
							setEdgeState(r-1, c, RIGHT, WHITE); // >^
							setEdgeState(r, c, RIGHT, WHITE); // >.
						}
						if(isBlack(r, c, RIGHT)) // >.
						{
							setEdgeState(r-1, c, RIGHT, WHITE); // >^
							setEdgeState(r, c+1, TOP, WHITE); // >>
						}
						break;
					case RIGHT:
						if(isBlack(r, c, BOTTOM)) // .>
						{
							setEdgeState(r+1, c, RIGHT, WHITE); // ..
							setEdgeState(r, c, BOTTOM, WHITE); // .<
						}
						if(isBlack(r+1, c, RIGHT)) // ..
						{
							setEdgeState(r, c+1, BOTTOM, WHITE); // .>
							setEdgeState(r, c, BOTTOM, WHITE); // .<
						}
						if(isBlack(r, c, BOTTOM)) // .<
						{
							setEdgeState(r, c+1, BOTTOM, WHITE); // .>
							setEdgeState(r+1, c, RIGHT, WHITE); // ..
						}
						break;
					case BOTTOM:
						if(isBlack(r+1, c, LEFT)) // <.
						{
							setEdgeState(r, c-1, BOTTOM, WHITE); // <<
							setEdgeState(r, c, LEFT, WHITE); // <^
						}
						if(isBlack(r, c-1, BOTTOM)) // <<
						{
							setEdgeState(r+1, c, LEFT, WHITE); // <.
							setEdgeState(r, c, LEFT, WHITE); // <^
						}
						if(isBlack(r, c, LEFT)) // <^
						{
							setEdgeState(r+1, c, LEFT, WHITE); // <.
							setEdgeState(r, c-1, BOTTOM, WHITE); // <<
						}
						break;
				}	
				
			}
}

void ruleProlongPath()
{
	int r, c;
	for(r=0; r<=rows; r++)
		for(c=0; c<=cols; c++)
		{
			int numBlack = 0;
			if( isBlack(r-1, c-1, BOTTOM) || isBlack(r, c-1, TOP) )
				numBlack++;
			if( isBlack(r-1, c-1, RIGHT) || isBlack(r-1, c, LEFT) )
				numBlack++;
			if( isBlack(r-1, c, BOTTOM) || isBlack(r, c, TOP) )
				numBlack++;
			if( isBlack(r, c-1, RIGHT) || isBlack(r, c, LEFT) )
				numBlack++;
		
			int numWhite = 0; // now count the impossible directions
			if( isWhite(r-1, c-1, BOTTOM) && isWhite(r, c-1, TOP) )
				numWhite++;
			if( isWhite(r-1, c-1, RIGHT) && isWhite(r-1, c, LEFT) )
				numWhite++;
			if( isWhite(r-1, c, BOTTOM) && isWhite(r, c, TOP) )
				numWhite++;
			if( isWhite(r, c-1, RIGHT) && isWhite(r, c, LEFT) )
				numWhite++;
				
			if(numBlack == 2 || numBlack == 0 && numWhite == 3) // the remaining edge must be white
			{ // RULE 9 + 10
				setEdgeState(r-1, c-1, BOTTOM, WHITE);
				setEdgeState(r, c-1, TOP, WHITE);
				setEdgeState(r-1, c, BOTTOM, WHITE);
				setEdgeState(r, c, TOP, WHITE);
				setEdgeState(r-1, c-1, RIGHT, WHITE);
				setEdgeState(r-1, c, LEFT, WHITE);
				setEdgeState(r, c-1, RIGHT, WHITE);
				setEdgeState(r, c, LEFT, WHITE);
			}
			else if(numBlack == 1 && numWhite == 2) // the remaining edge must be black
			{ // RULE 8
				// one of these will be switched from pending to black				
				setEdgeState(r-1, c-1, BOTTOM, BLACK);
				setEdgeState(r, c-1, TOP, BLACK);
				setEdgeState(r-1, c, BOTTOM, BLACK);
				setEdgeState(r, c, TOP, BLACK);
				setEdgeState(r-1, c-1, RIGHT, BLACK);
				setEdgeState(r-1, c, LEFT, BLACK);
				setEdgeState(r, c-1, RIGHT, BLACK);
				setEdgeState(r, c, LEFT, BLACK);
			}
		}
}

void ruleFixTwoWithIngressBlockedExit() // RULE 16
{
	int r, c;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges != 2)
				continue;
				
			if( isBlack(r-1, c, LEFT) && isWhite(r, c-1, TOP) || isWhite(r-1, c, LEFT) && isBlack(r, c-1, TOP) )
			{
				if(isWhite(r+1, c, RIGHT))
					setEdgeState(r, c+1, BOTTOM, BLACK);
				if(isWhite(r, c+1, BOTTOM))
					setEdgeState(r+1, c, RIGHT, BLACK);
			}
			if( isBlack(r-1, c, RIGHT) && isWhite(r, c+1, TOP) || isWhite(r-1, c, RIGHT) && isBlack(r, c+1, TOP) )
			{
				if(isWhite(r+1, c, LEFT))
					setEdgeState(r, c-1, BOTTOM, BLACK);
				if(isWhite(r, c-1, BOTTOM))
					setEdgeState(r+1, c, LEFT, BLACK);
			}
			if( isBlack(r+1, c, RIGHT) && isWhite(r, c+1, BOTTOM) || isWhite(r+1, c, RIGHT) && isBlack(r, c+1, BOTTOM) )
			{
				if(isWhite(r-1, c, LEFT))
					setEdgeState(r, c-1, TOP, BLACK);
				if(isWhite(r, c-1, TOP))
					setEdgeState(r-1, c, LEFT, BLACK);
			}
			if( isBlack(r+1, c, LEFT) && isWhite(r, c-1, BOTTOM) || isWhite(r+1, c, LEFT) && isBlack(r, c-1, BOTTOM) )
			{
				if(isWhite(r-1, c, RIGHT))
					setEdgeState(r, c+1, TOP, BLACK);
				if(isWhite(r, c+1, TOP))
					setEdgeState(r-1, c, RIGHT, BLACK);
			}
		}
}

void ruleFixThreeWithIngressBlockedExit() // RULE 14
{
	int r, c;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges != 3)
				continue;
				
			if( isBlack(r-1, c, LEFT) && isWhite(r, c-1, TOP) || isWhite(r-1, c, LEFT) && isBlack(r, c-1, TOP) )
			{
				setEdgeState(r, c, RIGHT, BLACK);
				setEdgeState(r, c, BOTTOM, BLACK);
			}
			if( isBlack(r-1, c, RIGHT) && isWhite(r, c+1, TOP) || isWhite(r-1, c, RIGHT) && isBlack(r, c+1, TOP) )
			{
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r, c, BOTTOM, BLACK);
			}
			if( isBlack(r+1, c, RIGHT) && isWhite(r, c+1, BOTTOM) || isWhite(r+1, c, RIGHT) && isBlack(r, c+1, BOTTOM) )
			{
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r, c, TOP, BLACK);
			}
			if( isBlack(r+1, c, LEFT) && isWhite(r, c-1, BOTTOM) || isWhite(r+1, c, LEFT) && isBlack(r, c-1, BOTTOM) )
			{
				setEdgeState(r, c, RIGHT, BLACK);
				setEdgeState(r, c, TOP, BLACK);
			}
		}
}

void ruleFixThreeWithBlockedExit() // RULE 11
{
	int r, c;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges != 3)
				continue;
			if( isWhite(r, c-1, TOP) && isWhite(r-1, c, LEFT) )
			{
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r, c, TOP, BLACK);
			}
			if( isWhite(r, c+1, TOP) && isWhite(r-1, c, RIGHT) )
			{
				setEdgeState(r, c, RIGHT, BLACK);
				setEdgeState(r, c, TOP, BLACK);
			}
			if( isWhite(r, c+1, BOTTOM) && isWhite(r+1, c, RIGHT) )
			{
				setEdgeState(r, c, RIGHT, BLACK);
				setEdgeState(r, c, BOTTOM, BLACK);
			}
			if( isWhite(r, c-1, BOTTOM) && isWhite(r+1, c, LEFT) )
			{
				setEdgeState(r, c, LEFT, BLACK);
				setEdgeState(r, c, BOTTOM, BLACK);
			}
		}
}

void ruleFixTwoWithBlockedExit() // RULE 12
{
	int r, c;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges != 2)
				continue;
			if( isWhite(r, c-1, TOP) && isWhite(r-1, c, LEFT) )
			{
				if(isWhite(r, c-1, BOTTOM))
					setEdgeState(r+1, c, LEFT, BLACK);
				if(isWhite(r+1, c, LEFT))					
					setEdgeState(r, c-1, BOTTOM, BLACK);
				if(isWhite(r, c+1, TOP))
					setEdgeState(r-1, c, RIGHT, BLACK);
				if(isWhite(r-1, c, RIGHT))					
					setEdgeState(r, c+1, TOP, BLACK);
			}
			if( isWhite(r, c+1, TOP) && isWhite(r-1, c, RIGHT) )
			{
				if(isWhite(r, c-1, TOP))
					setEdgeState(r-1, c, LEFT, BLACK);
				if(isWhite(r-1, c, LEFT))					
					setEdgeState(r, c-1, TOP, BLACK);
				if(isWhite(r, c+1, BOTTOM))
					setEdgeState(r+1, c, RIGHT, BLACK);
				if(isWhite(r+1, c, RIGHT))					
					setEdgeState(r, c+1, BOTTOM, BLACK);
			}
			if( isWhite(r, c+1, BOTTOM) && isWhite(r+1, c, RIGHT) )
			{
				if(isWhite(r, c-1, BOTTOM))
					setEdgeState(r+1, c, LEFT, BLACK);
				if(isWhite(r+1, c, LEFT))					
					setEdgeState(r, c-1, BOTTOM, BLACK);
				if(isWhite(r, c+1, TOP))
					setEdgeState(r-1, c, RIGHT, BLACK);
				if(isWhite(r-1, c, RIGHT))					
					setEdgeState(r, c+1, TOP, BLACK);
			}
			if( isWhite(r, c-1, BOTTOM) && isWhite(r+1, c, LEFT) )
			{
				if(isWhite(r, c-1, TOP))
					setEdgeState(r-1, c, LEFT, BLACK);
				if(isWhite(r-1, c, LEFT))					
					setEdgeState(r, c-1, TOP, BLACK);
				if(isWhite(r, c+1, BOTTOM))
					setEdgeState(r+1, c, RIGHT, BLACK);
				if(isWhite(r+1, c, RIGHT))					
					setEdgeState(r, c+1, BOTTOM, BLACK);
			}
		}
}

void ruleFixOneWithBlockedExit() // RULE 13
{
	int r, c;
	for(r=0; r<rows; r++)
		for(c=0; c<cols; c++)
		{
			if(cell[r][c].numEdges != 1)
				continue;
			if( isWhite(r, c-1, TOP) && isWhite(r-1, c, LEFT) )
			{
					setEdgeState(r, c, LEFT, WHITE);
					setEdgeState(r, c, TOP, WHITE);
			}
			if( isWhite(r, c+1, TOP) && isWhite(r-1, c, RIGHT) )
			{
				setEdgeState(r, c, RIGHT, WHITE);
				setEdgeState(r, c, TOP, WHITE);
			}
			if( isWhite(r, c+1, BOTTOM) && isWhite(r+1, c, RIGHT) )
			{
					setEdgeState(r, c, RIGHT, WHITE);
					setEdgeState(r, c, BOTTOM, WHITE);
			}
			if( isWhite(r, c-1, BOTTOM) && isWhite(r+1, c, LEFT) )
			{
					setEdgeState(r, c, LEFT, WHITE);
					setEdgeState(r, c, BOTTOM, WHITE);
			}
		}
}

void solve()
{

	ruleZeroFour();
	ruleCornerElements();
	ruleNeighborThree();
	do
	{
		isRoundSuccessful = false;
		ruleFixRemaining();
		ruleFixOneWithBlockedExit();
		ruleFixTwoWithBlockedExit();
		ruleFixTwoWithIngressBlockedExit();
		ruleFixThreeWithBlockedExit();
		ruleFixThreeWithIngressBlockedExit();
		ruleRemoveWrongTurns();
		ruleProlongPath();
		
	} while(isRoundSuccessful);

	print(1);
}

int main(int argc, char** args)
{
	int i, j;
	int c = 0;
	while(true)
	{
		scanf("%d %d\n", &rows, &cols);
		if(rows == 0 || cols == 0)
			break;
			
		for(i=0; i<rows;i++)
			for(j=0; j<cols; j++)
			{
				scanf("%d", &cell[i][j].numEdges);
				// reset edge states
				cell[i][j].edge[LEFT] = PENDING;
				cell[i][j].edge[RIGHT] = PENDING;
				cell[i][j].edge[TOP] = PENDING;
				cell[i][j].edge[BOTTOM] = PENDING;
			}
			
		printf("%d\n", ++c);
		solve();
	}

	return 0;
}

