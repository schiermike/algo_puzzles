/**
 * https://www.spoj.pl/problems/COINS/
 * Schier Michael
 * Strategy: hash results, always divide for x >= 12
 */

#include <stdio.h>
#include <string.h>

#define bool int
#define false 0
#define true !false

#define HASHMOD 1000003

typedef struct
{
	unsigned int key;
	unsigned int value;
	unsigned int next;
} HashItem;

typedef struct
{
	unsigned int size;
	unsigned int toPos[HASHMOD];
	HashItem table[HASHMOD];
} Hash;

Hash hash;

void clear()
{
	hash.size = 0;
	memset(&hash.toPos, 0, HASHMOD * sizeof(unsigned int));
	memset(&hash.table, 0, HASHMOD * sizeof(HashItem));
}

unsigned int getValue(unsigned int key)
{
	unsigned int h = key % HASHMOD;
	if(hash.toPos[h] == 0)
		return -1;
		
	unsigned int p; // browse hash entries
	for(p = hash.toPos[h]; p != 0; p = hash.table[p].next)
		if(hash.table[p].key == key)
			return hash.table[p].value;

	return -1;
}

void putValue(unsigned int key, unsigned int value)
{
	unsigned int h = key % HASHMOD;
	
	hash.size++;
	unsigned int p = hash.toPos[h];
	hash.toPos[h] = hash.size;
	hash.table[hash.size] = (HashItem){key, value, p};
}

unsigned int splitWin(unsigned int n)
{
	if(n<24)
	{
		if(n == 12 || n == 16 || n == 18 || n == 20 || n == 21 | n == 22)
			return 1;
		return 0;
	}
	
	unsigned int n2 = n/2;
	unsigned int v2 = getValue(n2);
	if(v2 == -1)
	{
		v2 = splitWin(n2);
		putValue(n2, v2);
	}
		
	unsigned int n3 = n/3;
	unsigned int v3 = getValue(n3);
	if(v3 == -1)
	{
		v3 = splitWin(n3);
		putValue(n3, v3);
	}
		
	unsigned int n4 = n/4;
	unsigned int v4 = getValue(n4);
	if(v4 == -1)
	{
		v4 = splitWin(n4);
		putValue(n4, v4);
	}
	
	return n2 + n3 + n4 - n + v2 + v3 + v4;
}

int main(int argc, char** args)
{
	unsigned int n;
	while(scanf("%u", &n) != EOF)
	{
		clear();
		printf("%u\n", n+splitWin(n));
	}

	return 0;
}

