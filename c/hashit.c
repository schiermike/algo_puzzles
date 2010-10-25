#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 101
#define HASH_FACTOR 19
#define MAX_COLLISIONS 20

char** hashTable;
char lineBuffer[20];

void clearHashTable()
{
	int i;
	for(i = 0; i < MAX_SIZE; i++)
		hashTable[i] = NULL;
}

void printHashTable()
{
	int entryCount = 0;
	int i;
	for(i = 0; i < MAX_SIZE; i++)
		if(hashTable[i] != NULL)
			entryCount++;
	printf("%d\n", entryCount);
	for(i = 0; i < MAX_SIZE; i++)
		if(hashTable[i] != NULL)
			printf("%d:%s\n", i, hashTable[i]);
}

int getHash(const char* value, int j)
{
	int n = 0;
	int i;
	const char* v = value;
	for(i = 1; *v != '\0'; i++, v++)
		n += *v * i;

	n = (HASH_FACTOR*n + j*j + 23*j) % MAX_SIZE;
	return n;
}

void addEntry(const char* value)
{
	int j;
	// first check ...
	for(j = 0; j < MAX_COLLISIONS; j++)
	{
		int pos = getHash(value, j);
		if(hashTable[pos] == NULL)
			continue;
		if(strcmp(hashTable[pos], value) == 0)
		{
#ifdef DEBUG
			printf("Element '%s' already in table at position %d\n", value, pos);
#endif
			return;
		}
	}
	// ... then insert
	for(j = 0; j < MAX_COLLISIONS; j++)
	{
		int pos = getHash(value, j);
		if(hashTable[pos] != NULL)
			continue;
		hashTable[pos] = (char*)malloc((strlen(value)+1)*sizeof(char));
		strcpy(hashTable[pos], value);
#ifdef DEBUG
		printf("Adding '%s' at position %d\n", value, pos);
#endif
		return;
	}
#ifdef DEBUG
	printf("Failed to insert '%s' - no more space left!\n", value);
#endif
}

void removeEntry(const char* value)
{
	int j;
	for(j = 0; j < MAX_COLLISIONS; j++)
	{
		int pos = getHash(value, j);
		if(hashTable[pos] == NULL)
			continue;
		if(strcmp(value, hashTable[pos]) != 0)
			continue;
		free(hashTable[pos]);
		hashTable[pos] = NULL;
#ifdef DEBUG
		printf("Removed '%s' from position %d\n", value, pos);
#endif
		return;
	}
#ifdef DEBUG
	printf("No such element found: '%s'\n", value);
#endif
}

void testcase()
{
	clearHashTable();

	int numOperations;
	scanf("%d\n", &numOperations);
	while(numOperations-->0)
	{
		scanf("%s\n", lineBuffer);
		if(lineBuffer[0] == 'A')
			addEntry(lineBuffer+4);
		else
			removeEntry(lineBuffer+4);
	}

	printHashTable();
}

int main(int argc, char** args)
{
	hashTable = (char**)malloc(MAX_SIZE*sizeof(char**));

	int t;
	scanf("%d\n", &t);
	while(t-->0)
		testcase();

	return 0;
}

