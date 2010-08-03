/**
 * https://www.spoj.pl/problems/ADDREV/
 * Schier Michael
 */

#include <stdio.h>
#define bool char
#define false 0
#define true !false

char in1[11];
char in2[11];
char out[11];

void reverse(char* s)
{
	if(*s == 0)
		return;

	char* e = s;
	while(*(e+1) != '\0')
		e++;
	while(*e == '0')
		e--;
		
	*(e+1) = '\0';
		
	char t;
	while(e - s > 0)
	{
		t = *s;
		*s = *e;
		*e = t;
		s++;
		e--;
	}
}

int main()
{
	int n;
	scanf("%d\n", &n);
	while(n-->0)
	{
		scanf("%s %s\n", in1, in2);

		reverse(in1);
		int a;
		sscanf(in1, "%d", &a);
		
		reverse(in2);
		int b;
		sscanf(in2, "%d", &b);
		
		int c = a+b;
		
		sprintf(out, "%d", c);
		reverse(out);
		
		printf("%s\n", out);
	}

	return 0;
}

