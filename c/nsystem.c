#include <stdio.h>
#include <stdlib.h>

char buffer[10];

int parseNumber()
{
	char* c = buffer;
	int n = 0;
	int f = 1;
	scanf("%s", c);
	while(*c != '\0')
	{
		switch(*c)
		{
			case 'm':
				n += f * 1000;
				f = 1;
				break;
			case 'c':
				n += f * 100;
				f = 1;
				break;
			case 'x':
				n += f * 10;
				f = 1;
				break;
			case 'i':
				n += f;
				f = 1;
				break;
			default:
				f = *c - '0';
				break;
		}
		c++;
	}
	return n;
}

void printNumber(int n)
{
	int f = n/1000;
	n = n % 1000;
	if(f > 1)
		printf("%d", f);
	if(f >= 1)
		printf("m");
	f = n / 100;
	n = n % 100;
	if(f > 1)
		printf("%d", f);
	if(f >= 1)
		printf("c");
	f = n / 10;
	n = n % 10;
	if(f > 1)
		printf("%d", f);
	if(f >= 1)
		printf("x");
	f = n;
	if(f > 1)
		printf("%d", f);
	if(f >= 1)
		printf("i");
	printf("\n");
}

int main(int argc, char** args)
{
	int numCases;
	scanf("%d\n", &numCases);
	while(numCases-->0)
	{
		int n1 = parseNumber();
		int n2 = parseNumber();
		printNumber(n1+n2);
	}
	return 0;
}

