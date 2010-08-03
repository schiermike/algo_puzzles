/**
 * Test case generator for lc2
 * Schier Michael
 */
 
#include <stdlib.h>
#include <stdio.h>

int main()
{
	srand(time(NULL));
	
	int lines = 10;
	while(lines-->0)
	{
		int lineLen = 100000;
		while(lineLen-->0)
			putchar(97 + rand()%26);
		putchar('\n');
	}

	return 0;
}

