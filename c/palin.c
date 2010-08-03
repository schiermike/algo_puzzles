/**
 * https://www.spoj.pl/problems/PALIN/
 * Schier Michael
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char input[1000000];

// 2233 -> 2332
// 2213 -> 2222
// 2133 -> 2222
// 2033 -> 2112
// 8999 -> 9009
void processInput()
{
	unsigned int len = strlen(input);
	unsigned int middle = len/2 - 1 + (len%2);
	int i = middle;
	int mode = 0;
	while(i >= 0)
	{
		if(mode == 0)
		{
			if(input[i] > input[len - i - 1])
				mode = 1;
			if(input[i] < input[len - i - 1])
				mode = 2;
		}
		input[len - i - 1] = input[i];
		i--;
	}
	if(mode == 0)
		mode = 2;

	if(mode == 2) // created palindrome is smaller than the input
	{
		i = middle;
		while(i >= 0)
		{
			input[i] = input[i] == '9' ? '0' : input[i]+1;
			input[len - i - 1] = input[i];
			if(input[i] != '0')
				break;
			i--;
		}
		if(i == -1) // special case: 99999xxxxx
		{
			printf("1");
			input[len-1] = '1';
		}
	}
	
	printf("%s\n", input);
}

int main(int argc, char** argv)
{
	unsigned int numTests;
	scanf("%u\n", &numTests);

	unsigned int i;
	for(i = 0; i < numTests; i++)
	{
		scanf("%s\n", input);
		processInput();
	}

	return 0;
}

