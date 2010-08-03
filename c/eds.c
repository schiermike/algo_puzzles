/**
 * https://www.spoj.pl/problems/EDS/
 * Schier Michael
 */

#include <stdio.h>

#define bool char
#define false 0
#define true !false

bool perms[26][26];

void resetPerm(char letter)
{
	int j;
	for(j=0; j<26; j++)
		perms[letter-65][j] = false;
}

void resetPerms()
{
	char c;
	for(c=65; c<=90; c++)
		resetPerm(c);
}

bool isUppercase(char letter)
{
	return letter >= 65 && letter <= 90;
}

bool isOperator(char letter)
{
	return letter == '=' || letter == '+' || letter == '-';
}

bool isLowercase(char letter)
{
	return letter >= 97 && letter <= 122;
}

void evalPerms(char* input)
{
	while(true)
	{
		char* op = input;
		while(!isOperator(*op))
			op++;
			
		while(isUppercase(*input))
		{
			if(*op == '=')
				resetPerm(*input);
			
			char* p = op+1;
			while(isLowercase(*p))
			{
				switch(*op)
				{
					case '+':
					case '=':
						perms[*input-65][*p-97] = true;
						break;
					case '-':
						perms[*input-65][*p-97] = false;
						break;
				}
				p++;
			}
			input++;
		}
		
		while(!isUppercase(*input))
		{
			if(*input == '\0')
				return;
			input++;
		}
	}
}

void printPerms(int caseNum)
{
	int tmpList[26];
	int tmpListLen;

	printf("%d:", caseNum);
	
	int i;
	for(i=0; i<26; i++)
	{
		tmpListLen = 0;
		int j;
		for(j=0; j<26; j++)
			if(perms[i][j])
				tmpList[tmpListLen++] = j;
		if(tmpListLen == 0)
			continue;
			
		printf("%c", i+65);
		// can we merge some entries?
		for(j=i+1; j<26; j++)
		{
			int k;
			int l = 0;
			bool check = true;
			for(k=0; k<26; k++)
			{
				if(perms[j][k])
				{
				 	if(l < tmpListLen && tmpList[l] == k)
				 		l++;
				 	else
				 		break;
				}
				else if(l < tmpListLen && tmpList[l] == k)
					check = false;			
			}
			
			if(k != 26 || l>0 && l!=tmpListLen)
				break;
				
			if(check && l==tmpListLen)
				printf("%c", j+65);
		}
		i = j-1;
		
		for(j=0; j<tmpListLen; j++)
				printf("%c", tmpList[j]+97);
			
	}
	
	printf("\n");
}

int main()
{
	char input[80];
	int caseNum = 1;
	while(scanf("%s\n", input) == 1 && input[0] != '#')
	{
		resetPerms();
		evalPerms(input);
		printPerms(caseNum);
		caseNum++;
	}



	return 0;
}

