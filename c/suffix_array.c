#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define false 0
#define true (!false)

#define MAX_INPUT_LEN 250000
char input[MAX_INPUT_LEN+1];
int inputLen;

int* suffixArray;
int* sortBuffer;

#ifdef DEBUG
void printSuffixArray()
{
	int i;
	for(i = 0; i < inputLen; i++)
		printf("%s\n", input + suffixArray[i]);
}
#endif

void sort(int first, int last)
{
	if(first + 1 == last)
	{
		if( strcmp(input + suffixArray[first], input + suffixArray[last]) > 0)
		{
			int tmp = suffixArray[first];
			suffixArray[first] = suffixArray[last];
			suffixArray[last] = tmp;
		}
		return;
	}

	int middle = (first + last) / 2;
	if(first < middle)
		sort(first, middle);
	if(middle+1 < last)
		sort(middle+1, last);
	// merge results

	int i = first;
	int j = middle+1;
	int k = 0;
	while(true)
	{
		if( i <= middle && (j>last || strcmp(input + suffixArray[i] , input + suffixArray[j]) <= 0) )
		{
			sortBuffer[k] = suffixArray[i];
			i++;
		}
		else if( j <= last)
		{
			sortBuffer[k] = suffixArray[j];
			j++;
		}
		else
			break;
		k++;
	}

	// copy the merge stuff into the suffixArray
	for(i = 0; i < k; i++)
	{
		suffixArray[first + i] = sortBuffer[i];
	}
}

// do a quick sort on the suffix array
void sortSuffixArray()
{
	sortBuffer = (int*)malloc(sizeof(int)*inputLen);
	sort(0, inputLen-1);
}

int main(int argc, char** args)
{
	scanf("%s", input);
	inputLen = strlen(input);

	// initialize the unsorted suffix array
	suffixArray = (int*)malloc(sizeof(int)*inputLen);
	int i;
	for(i = 0; i < inputLen; i++)
		suffixArray[i] = i;

	sortSuffixArray();	

#ifdef DEBUG
	printSuffixArray();
#endif

	return 0;
}
