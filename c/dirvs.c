/**
 * https://www.spoj.pl/problems/DIRVS/
 * Schier Michael
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define null NULL
#define bool int
#define true 1
#define false 0

typedef struct
{
	int row;
	int col;
} Point;

typedef struct __llitem
{
	Point p;
	struct __llitem* next;
} LLItem;

// read from input
int rows, cols;
int height[200][200];
Point p1;
Point p2;
// ------------------

// to some points, their distance to p1 will be assigned. -1 means non-assignment
int distance[200][200];
// the queue for the dijkstra-algorithm - the elements are ordered in ascending order (distance to p1)
LLItem* queue;

void readCase()
{
	int i, j;
	scanf("%i %i\n", &rows, &cols);
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
		{
			distance[i][j] = -1;
			scanf("%i", &height[i][j]);
		}
	scanf("%i %i %i %i\n", &p1.row, &p1.col, &p2.row, &p2.col);
	p1.row--;
	p1.col--;
	p2.row--;
	p2.col--;
}

// frees unused memory allocated by the algorithm
void clearQueue()
{
	LLItem* tmp;
	while(queue)
	{
		queue = queue->next;
		free(queue);
	}
}

// adds the point with the given coordinates to the queue
// its distance from p1 must be known
void queuePoint(int row, int col)
{
	LLItem* newItem = (LLItem*)malloc(sizeof(LLItem));
	newItem->p.row = row;
	newItem->p.col = col;
	newItem->next = null;
	
	// list was empty
	if(queue == null)
	{
		queue = newItem;
		return;
	}
	
	int d = distance[row][col];
	// add as first element
	if(distance[queue->p.row][queue->p.col] > d)
	{
		newItem->next = queue;
		queue = newItem;
		return;
	}
	
	// traverse queue to find right position
	LLItem* tmp = queue;
	while(tmp->next != null)
	{
		// insert somewhere in the queue
		if(distance[tmp->next->p.row][tmp->next->p.col] > d)
		{
			newItem->next = tmp->next;
			tmp->next = newItem;
			return;
		}
		tmp = tmp->next;
	}
	
	// distance is bigger than that of all elements of the queue -> append at the end
	tmp->next = newItem;
	
}

// removes the first entry from the queue and frees its memory
Point dequeuePoint()
{
	Point p = queue->p;
	LLItem* tmp = queue;
	queue = queue->next;
	free(tmp);
	return p;
}

int abs(int value)
{
	return value >= 0 ? value : -value;
}

// check whether those points can see each other
bool canSeeEachOther(int row1, int col1, int row2, int col2)
{
	int h1 = height[row1][col1];
	int h2 = height[row2][col2];

	int deltaR = row2 - row1;
	int deltaC = col2 - col1;
	int deltaH = h2 - h1;

	// traverse from point1 to point2 and stop at each intersection with the x,y grid where we check the height
	double p1R = row1 + 0.5;
	double p1C = col1 + 0.5;
	double skalar = 0;
	int nextRHit = deltaR >= 0 ? row1 + 1 : row1;
	int nextCHit = deltaC >= 0 ? col1 + 1 : col1;
	
	while(true)
	{
		double skalarR = deltaR != 0 ? (nextRHit - p1R) / deltaR : 1000;
		double skalarC = deltaC != 0 ? (nextCHit - p1C) / deltaC : 1000;
		
		skalar = skalarR < skalarC ? skalarR : skalarC;
		// termination condition: we only check until we pass point p2
		if(skalar > 1)
			break;
			
		double currentH = 0.5 + h1 + skalar * deltaH;
		
		if(skalarR < skalarC) // we hit the row border before the col border
		{
			int currentC = deltaC >= 0 ? nextCHit - 1 : nextCHit;
			if(currentH < height[nextRHit-1][currentC])
				return false;
			if(currentH < height[nextRHit][currentC])
				return false;
				
			nextRHit += deltaR >= 0 ? 1 : -1;
		}
		else if(skalarR > skalarC) // we hit the col border before the row border
		{
			int currentR = deltaR >= 0 ? nextRHit - 1 : nextRHit;
			if(currentH < height[currentR][nextCHit-1])
				return false;
			if(currentH < height[currentR][nextCHit])
				return false;
		
			nextCHit += deltaC >= 0 ? 1 : -1;
		}
		else // we hit the center of 4 boxes
		{
			if(deltaR*deltaC >= 0) // left-bottom <-> right-top
			{
				if(currentH < height[nextRHit-1][nextCHit-1])
					return false;
				if(currentH < height[nextRHit][nextCHit])
					return false;
			}
			else // left-top <-> right-bottom
			{
				if(currentH < height[nextRHit-1][nextCHit])
					return false;
				if(currentH < height[nextRHit][nextCHit-1])
					return false;
			}
		
			nextRHit += deltaR >= 0 ? 1 : -1;
			nextCHit += deltaC >= 0 ? 1 : -1;
		}
	}

	return true;
}

bool tryStep(int row, int col, int prevHeight, int dist)
{
	// check validity
	if(row < 0 || row >= rows || col < 0 || col >= cols)
		return false;

	// we already visited this location 
	// (due to the algorithm, we cannot find a shorter path to this position)
	if(distance[row][col] >= 0)
		return false;
		
	// check the step height
	int deltaHeight = height[row][col] - prevHeight;
	if( deltaHeight < -3 || deltaHeight > 1)
		return false;
		
	// check visibility from p1 and p2
	if(!canSeeEachOther(p1.row, p1.col, row, col) && !canSeeEachOther(p2.row, p2.col, row, col))
		return false;
		
	// store distance and queue point
	distance[row][col] = dist;
	queuePoint(row, col);
	return row == p2.row && col == p2.col;
}

void pathSearch()
{
	while(queue != null)
	{
		Point p = dequeuePoint();
		int d = distance[p.row][p.col]+1;
		int h = height[p.row][p.col];
		
		if( tryStep(p.row-1, p.col, h, d) )
			break;
		if( tryStep(p.row+1, p.col, h, d) )
			break;
		if( tryStep(p.row, p.col-1, h, d) )
			break;
		if( tryStep(p.row, p.col+1, h, d) )
			break;
	}
}

void solveCase()
{
	distance[p1.row][p1.col] = 0;
	queuePoint(p1.row, p1.col);
	
	pathSearch();
	
	int dist = distance[p2.row][p2.col];
	if(dist == -1)
		printf("Mission impossible!\n");
	else
		printf("The shortest path is %i steps long.\n", dist);

	clearQueue();
}

int main(int argc, char** args)
{
	queue = null;

	int t;
	scanf("%i\n", &t);
	while(t-->0)
	{
		readCase();
		solveCase();
	}
	return 0;
}

