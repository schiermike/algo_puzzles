#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool
#define false 0
#define true (!false)

#define MAX_INPUT_LEN 250000
char input[MAX_INPUT_LEN+1];
int inputLen;

typedef struct _node
{
	int first;
	int last;
	struct _node* next[26];
} Node;

Node* newNode()
{
	Node* n = (Node*)malloc(sizeof(Node));
	n->first = -1;
	n->last = -1;
	int i;
	for(i=0; i<26; i++)
		n->next[i] = NULL;
}

#ifdef DEBUG
void printTree(Node* n, int level)
{
	static char buffer[MAX_INPUT_LEN+1];

	int i;
	for(i = 0; i < level; i++)
		printf("*");
	if(n->last >= 0)
		strncpy(buffer, input+n->first, n->last - n->first + 1);
	buffer[n->last+1] = '\0';
	printf("%s [%d..%d]\n", buffer, n->first, n->last);
	for(i = 0; i < 26; i++)
		if(n->next[i] != NULL)
			printTree(n->next[i], level+1);
}
#endif

void insert(Node* n, int i)
{
	while(true)
	{
		int j;
		for(j = n->first; i < inputLen && j <= n->last; i++, j++)
		{
			if(input[i] == input[j])
				continue;

			// we have to split up the node
			Node* child = newNode();
			child->first = j;
			child->last = n->last;
			n->last = j-1;
			for(j = 0; j < 26; j++)
			{
				// move all links to the child node
				child->next[j] = n->next[j];
				n->next[j] = NULL;
			}

			// create the remaining two links...
			// ... to the old node which has been moved downwards
			n->next[input[child->first]-'a'] = child;

			// ... to the new node
			n->next[input[i]-'a'] = newNode();
			n->next[input[i]-'a']->first = i;
			n->next[input[i]-'a']->last = inputLen-1;

			return; // we are done, we've inserted the string somewhere in the tree
		}

		if(i == inputLen)
		{
#ifdef DEBUG
			printf("no changes to the tree\n");
#endif
			return; // we are done, the string to insert ends here, no changes to the tree necessary
		}

		// the node string ends here
		int k = input[i] - 'a';
		if(n->next[k] == NULL) // there are no more child nodes
		{
			n->next[k] = newNode();
			n->next[k]->first = i;
			n->next[k]->last = inputLen-1;
			return; // we are done, we've inserted the whole string
		}
		else // we descend to the corresponding lower node
		{
			n = n->next[k];
		}
	}
}

int main(int argc, char** args)
{
	scanf("%s", input);
	inputLen = strlen(input);

	Node* root = newNode();
	root->first = 0;
	root->last = inputLen-1;

	int i;
	for(i=1; i<inputLen; i++)
		insert(root, i);

#ifdef DEBUG
	printTree(root, 0);
#endif

	return 0;
}
