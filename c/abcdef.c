/**
 * https://www.spoj.pl/problems/ABCDEF/
 * Schier Michael
 *
 * Equation: ( a * b + c ) / d - e = f
 * rewrite to: a * b + c = d * (e + f)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

// huge prime modules: 999983 5000011 9999991 99999989
// -3*10^4 <= a,b,c,d,e,f <= 3*10^4
// 1 <= size <= 100

#define HUGEPRIME 400009
#define HUGEPRIMEMAXDUP 20

int32_t s[100];
u_int32_t size;

typedef struct
{
	u_int32_t module;
	u_int32_t maxDups;

	u_int32_t numPlaces;
	u_int32_t numEntries;
	u_int32_t* idx2hash;	
	u_int64_t* hash2count;
	int32_t* hash2value;
} IntHashMap;

void init(IntHashMap* m, u_int32_t module, u_int32_t maxDups)
{
	m->module = module;
	m->maxDups = maxDups;

	m->numPlaces = 0;
	m->numEntries = 0;
	m->idx2hash = (u_int32_t*)calloc(module, sizeof(u_int32_t));
	m->hash2count = (u_int64_t*)calloc(module*(maxDups+1), sizeof(u_int64_t));
	m->hash2value = (int32_t*)calloc(module*(maxDups+1), sizeof(int32_t));
	
	if(!m->idx2hash || !m->hash2count || !m->hash2value)
		err(-1, "Could not allocate memory!");
	
	memset(m->hash2count, 0, module*(maxDups+1)*sizeof(u_int64_t));
}

void clear(IntHashMap* m)
{
	 free(m->idx2hash);
	 free(m->hash2count);
	 free(m->hash2value);
}

u_int32_t calcHash(u_int32_t module, int32_t value)
{
	return value >= 0 ? (value % module) : (module - (value % module));
}
	
void insert(IntHashMap* m, int32_t value, u_int64_t count)
{
	u_int32_t hash = calcHash(m->module, value);
	
	u_int32_t c;
	u_int32_t h = hash;
	for(c = 0; c < m->maxDups; c++, h += m->module)
	{
		if(m->hash2count[h] == 0)
		{
			if(c == 0)
			{
				m->idx2hash[m->numPlaces] = hash;
				m->numPlaces++;
			}
			m->numEntries++;
			m->hash2value[h] = value;
		}
		
		if(m->hash2value[h] == value)
		{
			m->hash2count[h] += count;
			return;
		}
	}
	int x = 3;
	int y = 0;
	printf("%d\n", x/y);
	err(-1, "No more places left to store inserted value %d with hash %d!\n", value, hash);
}

u_int64_t get(IntHashMap* m, int32_t value)
{
	u_int32_t c;
	u_int32_t h = calcHash(m->module, value);
	for(c = 0; c < m->maxDups; c++, h+= m->module)
	{
		if(m->hash2count[h] == 0)
			return 0;
			
		if(m->hash2value[h] == value)
			return m->hash2count[h];
	}
	return 0;
}

// --------------------------------------------------------------------------------------

IntHashMap ab, abc, ef, def;

u_int64_t solve()
{
	int i, j, h;
	
	init(&ab, HUGEPRIME, HUGEPRIMEMAXDUP);
	for(i = 0; i < size; i++)
		for(j = i; j < size; j++)
			insert(&ab, s[i] * s[j], s[i]==s[j] ? 1 : 2);
#ifdef DEBUG
	printf("ab %d %d %.2f%%\n", ab.numPlaces, ab.numEntries, (((double)ab.numEntries)/ab.numPlaces - 1)*100);
#endif

	init(&abc, HUGEPRIME, HUGEPRIMEMAXDUP);
	for(i = 0; i < ab.numPlaces; i++)
	{
		for(h = ab.idx2hash[i]; ab.hash2count[h] > 0; h += ab.module)
		{
			int32_t value = ab.hash2value[h];
			u_int64_t count = ab.hash2count[h];
			for(j = 0; j < size; j++)
				insert(&abc, value + s[j], count);
		}
	}
	clear(&ab);
#ifdef DEBUG
	printf("abc %d %d %.2f%%\n", abc.numPlaces, abc.numEntries, (((double)abc.numEntries)/abc.numPlaces - 1)*100);
#endif

	
	init(&ef, 120011, 1); // > 4*3*10^4
	for(i = 0; i < size; i++)
		for(j = i; j < size; j++)
			insert(&ef, s[i] + s[j], s[i]==s[j] ? 1 : 2);
#ifdef DEBUG
	printf("ef %d %d %.2f%%\n", ef.numPlaces, ef.numEntries, (((double)ef.numEntries)/ef.numPlaces - 1)*100);
#endif

	init(&def, HUGEPRIME, HUGEPRIMEMAXDUP);
	for(i = 0; i < ef.numPlaces; i++)
	{
		for(h = ef.idx2hash[i]; ef.hash2count[h] > 0; h += ef.module)
		{
			int32_t value = ef.hash2value[h];
			u_int64_t count = ef.hash2count[h];
			for(j = 0; j < size; j++)
				if(s[j] != 0)
					insert(&def, value * s[j], count);
		}
	}
	clear(&ef);

#ifdef DEBUG	
	printf("def %d %d %.2f%%\n", def.numPlaces, def.numEntries, (((double)def.numEntries)/def.numPlaces - 1)*100);
#endif

	u_int64_t count = 0;
	for(i = 0; i < abc.numPlaces; i++)
	{
		int hash = abc.idx2hash[i];
		for(h = abc.idx2hash[i]; abc.hash2count[h] > 0; h += abc.module)
		{
			u_int64_t numOcc = abc.hash2count[h];
			int32_t value = abc.hash2value[h];
				
			u_int64_t numOcc2 = get(&def, value);
			if(numOcc2)
				count += numOcc * numOcc2;
		}
	}
	
	clear(&abc);
	clear(&def);
	return count;
}

int main(int argc, char** args)
{
	scanf("%d\n", &size);
	int i;
	for(i=0; i<size; i++)
		scanf("%d\n", &s[i]);
	printf("%llu\n", solve());
	
	return 0;
}

