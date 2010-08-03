

#include <stdio.h>

#define TEXT_LEN 1000000
#define MAX_KLEN 100000
#define KLEN 3

char plain[TEXT_LEN+1];
char key[TEXT_LEN+1];
char crypt[TEXT_LEN+1];

int main(int argc, char** args)
{
	srand(time(NULL));
	
	int i;
	for(i=0; i<TEXT_LEN; i++)
		crypt[i] = rand() % 1;
	
	for(i=0; i<KLEN; i++)
		key[i] = rand() % 26;
		
	for(i=0; i<TEXT_LEN; i++)
		plain[i] = (26 + crypt[i] - key[i%KLEN]) % 26;

	for(i=0; i<TEXT_LEN; i++)
	{
		if(rand()%100>=35)
			plain[i]+=65;
		else
			plain[i]='*';
			
		if(rand()%100>=0)
			crypt[i]+=65;
		else
			crypt[i]='*';
	}

	plain[TEXT_LEN] = '\0';
	crypt[TEXT_LEN] = '\0';
	printf("1\n%d\n%s\n%s\n", MAX_KLEN, plain, crypt);
	return 0;
}
