/**
 * https://www.spoj.pl/problems/AE4A/
 * Schier Michael
 *
 * stamp k: the picture we want to produce
 * stamp s: big johnny's stamp
 * stamp l: little johnny's stamp
 */

#include <stdio.h>
#include <stdlib.h>
#define bool char
#define false 0
#define true !false

typedef struct
{
	int dim;
	bool c[1000][1000];
} Stamp;

Stamp k, s;

int lDist; // is the distance between the black spots on stamp l one or two?
bool diff[1000]; // temporary memory to make a diff on two rows

//-----------------------------------------------------------------------------------------------------

void print(Stamp* stmp, char* description, bool all)
{
	printf("%s\n", description);
	
	int i, j;
	for(i=0; i<2 || all && i<stmp->dim; i++)
	{
		for(j=0; j<stmp->dim; j++)
			printf("%c", stmp->c[i][j] ? 'x' : '.');
		printf("\n");
	}
	printf("\n");
}

void rotateRight(Stamp* stmp)
{
	int h, w;
	bool tmp;
	for(h=0; h<stmp->dim/2 + stmp->dim%2; h++)
	{
		int he = stmp->dim - 1 - h;
		for(w=0; w<stmp->dim/2; w++)
		{
			int we = stmp->dim - 1 - w;
			tmp = stmp->c[h][w];
			stmp->c[h][w] = stmp->c[we][h];
			stmp->c[we][h] = stmp->c[he][we];
			stmp->c[he][we] = stmp->c[w][he];
			stmp->c[w][he] = tmp;
		}
	}
}

void readInput()
{
	scanf("%d %d\n", &s.dim, &k.dim);

	int i, j;
	char tmp[9];
	bool ljVertical;
	tmp[0] = getchar() - 48;
	tmp[1] = getchar() - 48;
	tmp[2] = getchar() - 48;
	getchar();
	tmp[3] = getchar() - 48;
	tmp[4] = getchar() - 48;
	tmp[5] = getchar() - 48;
	getchar();
	tmp[6] = getchar() - 48;
	tmp[7] = getchar() - 48;
	tmp[8] = getchar() - 48;
	getchar();

	if(tmp[0] + tmp[1] + tmp[2] == 1) // vertical with first and (second or third) row
	{
		ljVertical = true;
		lDist = tmp[3] + tmp[4] + tmp[5] == 1 ? 1 : 2;
	}
	else if(tmp[0] + tmp[1] + tmp[2] == 2) // horizontal first row
	{
		ljVertical = false;
		lDist = tmp[1] == 1 ? 1 : 2;
	}
	else if(tmp[3] + tmp[4] + tmp[5] == 1) // vertical with second and third row
	{
		ljVertical = true;
		lDist = 1;
	}
	else if(tmp[3] + tmp[4] + tmp[5] == 2) // horizontal second row
	{
		ljVertical = false;
		lDist = tmp[4] == 1 ? 1 : 2;
	}
	else // horizontal third row
	{
		ljVertical = false;
		lDist = tmp[7] == 1 ? 1 : 2;	
	}

	for(i=0; i<s.dim; i++)
	{
		for(j=0; j<s.dim; j++)
			s.c[i][j] = getchar() - 48;
		getchar(); // \n
	}

	for(i=0; i<k.dim; i++)
	{
		for(j=0; j<k.dim; j++)
			k.c[i][j] = getchar() - 48;
		getchar(); // \n
	}
	
#ifdef DEBUG	
	printf("lDist is %d\n", lDist);
	print(&s, "Read stamp s from input:", true);
	print(&k, "Read stamp k from input:", true);
#endif
	
	if(!ljVertical)
	{
		rotateRight(&s);
		rotateRight(&k);
		
#ifdef DEBUG	
		print(&s, "Rotated stamp s:", true);
		print(&k, "Rotated stamp k:", true);
#endif
	}
}

/**
* Shrinks the given stamp to size 2 x stmp->dim
*/
void verticalShrink(Stamp* stmp)
{
	int h, w;
	for(h=stmp->dim-1; h>1; h--)
		for(w=0; w<stmp->dim; w++)
		{
			stmp->c[h-lDist][w] ^= stmp->c[h][w]; // flip bit if true
			stmp->c[h][w] = false;
		}
}

/**
* Applies simplified stamp s on simplified area k.
* hOff can either be 0 or 1
*/
void applyStamp(int hOff, int wOff)
{
	int w;
	for(w=0; w<s.dim; w++)
	{
		k.c[0][wOff+w] ^= s.c[hOff][w];
		k.c[1][wOff+w] ^= s.c[(hOff+1)%2][w];
	}
}

/**
* Shrinks the stamp k to size 2 x s.dim
*/
void horizontalShrink()
{
	int sOrigDim = s.dim;
	
	while(true)
	{
		int sRightMost = 0;
		int i;
		for(i=0; i<s.dim; i++)
			if(s.c[0][i] || s.c[1][i])
				sRightMost = i;
	
		int kRightMost = -1;
		for(i=0; i<k.dim; i++)
			if(k.c[0][i] || k.c[1][i])
				kRightMost = i;
				
		if(kRightMost == -1) // no black pixel left
			return;
		
#ifdef DEBUG
			printf("sRightMost=%d, kRightMost=%d\n", sRightMost, kRightMost);
#endif

		// we cannot reduce the problem any more, seems as if there's no solution to it
		if(kRightMost < sRightMost || k.dim - kRightMost < sOrigDim - sRightMost)
		{
#ifdef DEBUG
			printf("Stamp s doesn't fit into k appropriately! sRightMost=%d, kRightMost=%d\n", sRightMost, kRightMost);
#endif
			return;
		}
		else if(s.dim > sRightMost+1) // we reduce the dimension of s as far as possible
		{
			s.dim = sRightMost+1;
#ifdef DEBUG
			print(&s, "Horizontally shrinked stamp s!", false);
#endif
		}
	
		if( k.c[0][kRightMost] ^ k.c[1][kRightMost] ) // one black pixel at the right end of k
		{
			if( s.c[0][sRightMost] ^ s.c[1][sRightMost] ) // one black pixel at the right end of s
			{
				applyStamp(k.c[0][kRightMost] == s.c[0][sRightMost] ? 0 : 1, kRightMost - sRightMost);
			}
			else // two black pixels at the right end of s
			{
				/**
				* Simplify s which may look like this
				* 0110011  >  01111000
				* 0101101  >  01000100
				*/
				int lastDiff = -1;
				for(i=0; i<s.dim; i++)
				{
					diff[i] = s.c[0][i] ^ s.c[1][i];
					if(diff[i])
						lastDiff = i;
				}
				
				int delta = s.dim - 1 - lastDiff;
				if(sOrigDim + delta > k.dim) // we cannot apply this trick, s/k is too big/small
				{
#ifdef DEBUG
					printf("We cannot transform stamp s, it would exceed k's boundary!\n");
#endif
					return;
				}
				
				for(i=delta; i<=sRightMost; i++)
				{
					s.c[0][i] ^= diff[i-delta];
					s.c[1][i] ^= diff[i-delta];
				}
					
#ifdef DEBUG
				print(&s, "Transformed stamp s:", true);
#endif
				continue;
			}
		}
		else // two black pixels at the right end of k
		{
			if( s.c[0][sRightMost] ^ s.c[1][sRightMost] ) // one black pixel at the right end of s
			{
				applyStamp(0, kRightMost - sRightMost);
				applyStamp(1, kRightMost - sRightMost);
			}
			else // two black pixels at the right end of s
			{
				applyStamp(0, kRightMost - sRightMost);
			}
		}
		
#ifdef DEBUG
			print(&k, "Horizontally shrinked k:", false);
#endif
	}
}

/**
*  Checks if the vertically simplified stamp does not contain any black pixels
*/
bool isEmpty(Stamp* stmp)
{
	int i;
	for(i=0; i<stmp->dim; i++)
		if(stmp->c[0][i] || stmp->c[1][i])
			return false;
	return true;
}

bool solve()
{
	/**
	* The input procedure transformed the problem into one where we have a small stamp with black
	* pixels in the same column (vertical alignment).
	*/
	 
	/**
	* We now transform stamp s and k to only have black pixels in the first and second row, depending
	* on the pixel distance in the small stamp l.
	*/
	verticalShrink(&s);
	verticalShrink(&k);
#ifdef DEBUG
	print(&s, "Shrinked stamp s:", false);
	print(&k, "Shrinked stamp k:", false);
#endif

	if(isEmpty(&k)) // stamp k can be drawn by only using stamp l
		return true;
	if(isEmpty(&s)) // stamp k can not be drawn because it cannot be drawn simply by using stamp l and stamp s is empty
		return false;

	horizontalShrink();

	return isEmpty(&k);
}

int main()
{
	int t;
	scanf("%d\n", &t);
	while(t-->0)
	{
		readInput();
		
		printf("%s\n", solve() ? "TAK" : "NIE");
		
	}

	return 0;
}

