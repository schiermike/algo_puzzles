/**
 * https://www.spoj.pl/problems/CRYPTO4/
 * Schier Michael
 * partially known plaintext attack on Viginère
 */
 
#include <stdio.h>
#include <string.h>
#define bool char
#define false 0
#define true !false

#define MAXMSGLEN 1000000
#define MAXKEYLEN 100000
#ifdef DEBUG
	#define DEBUGLOG(fmt, args...) fprintf(stderr, "%d:\t"fmt , __LINE__ , ## args)
#else
	#define DEBUGLOG(fmt, args...)
#endif

 
int maxKeyLen;
int textLen;
char plain[MAXMSGLEN+1];
char crypt[MAXMSGLEN+1];
char key[MAXKEYLEN+1];

int numMatches;
int matchIdxList[MAXMSGLEN];
int numFixCandidates;
int fixCandidateIdxList[MAXMSGLEN];

bool firstPlainInsert;
bool firstFullKey;
int minKeyLenStep;

inline char decryptChar(char cypher, char keyOrPlain)
{
	return 65 + (26 + cypher - keyOrPlain) % 26;
}

inline void checkKeyLength(keyLen)
{
	int i;
	
	key[keyLen] = '\0';
	int keyDefCount = 0;
	for(i = 0; i < numMatches; i++)
	{
		int matchIdx = matchIdxList[i];
		int keyIdx = matchIdx % keyLen;

		char keyChar = decryptChar(crypt[matchIdx], plain[matchIdx]);
		if(key[keyIdx] == '*') // key not set until now
		{
			key[keyIdx] = keyChar;
			if(++keyDefCount == keyLen)
			{
				if(!firstFullKey)
					return;
			}
		}
		else if(key[keyIdx] != keyChar) // key conflict <-> invalid key length
			return;
	}
	
	if(keyDefCount == keyLen)
		firstFullKey = false;
	
	for(i = 0; i < numFixCandidates; i++)
	{
		int keyIdx = fixCandidateIdxList[i] % keyLen;
		// known key -> decrypt cypher
		if(key[keyIdx] != '*')
		{
			char plainChar = decryptChar(crypt[fixCandidateIdxList[i]], key[keyIdx]);
		
			if(firstPlainInsert)
				plain[fixCandidateIdxList[i]] = plainChar;
				
			if(plain[fixCandidateIdxList[i]] == plainChar)
				continue;
		}
		
		// unknown key at this place or decrypt mismatch -> forget about this place in the future
		plain[fixCandidateIdxList[i]] = '*';
		fixCandidateIdxList[i--]=fixCandidateIdxList[--numFixCandidates];
	}
	
	firstPlainInsert = false;
	
	static int tmp = 0;
	static int lastKeyLen = 0;
	static int keyLenCount = 0;
	static int minKeyLen = 0;
	if(tmp != numFixCandidates)
	{
		if(lastKeyLen != 0)
		{
			if(minKeyLen != lastKeyLen - keyLen)
			{
				minKeyLen = lastKeyLen - keyLen;
				keyLenCount = 0;
			}
			else if(keyLenCount >= 10)
			{
				if(minKeyLenStep == 0 || minKeyLenStep > lastKeyLen - keyLen)
				{
					minKeyLenStep = lastKeyLen - keyLen;
					DEBUGLOG("New minKeyLenStep: %d\n", minKeyLenStep);
				}
			}
		}
		keyLenCount++;
		lastKeyLen = keyLen;
	
		DEBUGLOG("CHECK keylen:%2d #%2d\n", keyLen, numFixCandidates);	
		tmp = numFixCandidates;
	}
//	DEBUGLOG("CHECK keylen:%2d #%2d key:%s\n", keyLen, numFixCandidates, key);	
//	DEBUGLOG("CHECK keylen:%2d #%2d plain:%s key:%s \n", keyLen, numFixCandidates, plain, key);
	
	return;
}

void fillGaps()
{
	numMatches = 0;
	numFixCandidates = 0;
	
	int i;
	for(i=0; i<textLen; i++)
	{
		if(crypt[i] != '*')
		{
			if(plain[i] == '*')
				fixCandidateIdxList[numFixCandidates++] = i;
			else
				matchIdxList[numMatches++] = i;
		}
	}
	if(numFixCandidates == 0 || numMatches == 0)
		return;
	
	firstPlainInsert = true;
	firstFullKey = true;
	int keyLen;
	minKeyLenStep = 0;
	for(keyLen = maxKeyLen; keyLen>maxKeyLen/2 && numFixCandidates > 0; keyLen-= minKeyLenStep == 0 ? 1 : minKeyLenStep)
	{
		memset(key, '*', keyLen); //non-set key means 255
		checkKeyLength(keyLen);
	}
}

int main(int argc, char** args)
{
	int t;
	scanf("%d\n", &t);
	while(t-->0)
	{
		scanf("%d\n", &maxKeyLen);
		scanf("%s\n", plain);
		scanf("%s\n", crypt);
		textLen = strlen(plain);
		if(maxKeyLen>textLen)
			maxKeyLen = textLen;
			
//		DEBUGLOG("PLAIN: %s\n", plain);
//		DEBUGLOG("CRYPT: %s\n", crypt);
		
		fillGaps();
		
		printf("%s\n", plain);
//		fprintf(stderr, "derived %d positions\n", numFixCandidates);
	}

	return 0;
}

