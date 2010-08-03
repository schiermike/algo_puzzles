/**
 * https://www.spoj.pl/problems/CMEXPR/
 * Schier Michael
 */
 
#include <stdio.h>

#define bool int
#define true 1
#define false 0

// input to read
unsigned char expression[250];
int len;

unsigned int openParenStack[125]; // stores the positions of openin parenthesis
unsigned char operatorStack[125]; // stores the inner operators of the corresponding parenthesis pairs: - overrides + overrides / overrides *
unsigned char lastOper; // helps when there are multiple closing parenthesis
int stackTop; // the size of the stack

// check whether we can remove this parenthesis pair
// preOper ( ...... innerOper ..... ) postOper
void closingParenthesis(int endPos)
{
	char innerOper = operatorStack[stackTop];
	operatorStack[stackTop] = 0;
	int startPos = openParenStack[--stackTop];
	char preOper = startPos > 0 ? expression[startPos-1] : 0;
	if(preOper == '(')
		preOper = 0;
	char postOper = endPos < len - 1 ? expression[endPos+1] : 0;
	if(postOper == ')')
		postOper = 0;
		
	if(innerOper != 0)
		lastOper = innerOper;
	else
		innerOper = lastOper;
		
	switch(innerOper)
	{
		case '-': // ( expr - expr )
		case '+': // ( expr + expr )
			if(preOper == '-' || preOper == '*' || preOper == '/' || postOper == '*' || postOper == '/')
				return;
			break;
		case '/': // ( expr / expr )
		case '*': // ( expr * expr )
			if(preOper == '/')
				return;
			break;
		default: // (x) or ((expr))
			break;
	}
	
	// passed the check -> remove parenthesis
	expression[startPos] = 0;
	expression[endPos] = 0;	
}

void transform()
{
	stackTop = 0;
	int i;
	for(i = 0; i < len; i++)
	{
		switch(expression[i])
		{
			case '(':
				openParenStack[stackTop++] = i;
				lastOper = 0;
				break;
			case ')':
				closingParenthesis(i);
				break;
			case '-':
				operatorStack[stackTop] = '-';
				break;
			case '+':
				if(operatorStack[stackTop] != '-')
					operatorStack[stackTop] = '+';
			case '/':
				if(operatorStack[stackTop] != '-' && operatorStack[stackTop] != '+')
					operatorStack[stackTop] = '/';
			case '*':
				if(operatorStack[stackTop] != '-' && operatorStack[stackTop] != '+' && operatorStack[stackTop] != '/')
					operatorStack[stackTop] = '*';
				break;
			default:
				break;
		}	
	}
}

void readInput()
{
	len = 0;
	unsigned char c;
	while(true)
	{
		c = getchar();
		if(c == '\n' || c == EOF)
			return;
		expression[len++] = c;
	}
}

void printOutput()
{
	int i;
	for(i = 0; i < len; i++)
		if(expression[i] != 0)
			putchar(expression[i]);
	putchar('\n');
}
 
int main(int argc, char** args)
{
	int t;
	scanf("%i\n", &t);
	while(t-->0)
	{
		readInput();
		transform();
		printOutput();
	}
	
	return 0;
}

