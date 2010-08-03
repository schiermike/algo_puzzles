
#include <stdlib.h>
#include <stdio.h>

char input[401];
unsigned int idx;

typedef struct _node
{
	char value;
	struct _node* left;
	struct _node* right;
} Node;

Node* createNode()
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->left = NULL;
	node->right = NULL;
	return node;
}

Node* buildParseTree()
{
	Node* node = createNode();

	if(input[idx] == '(')
	{
		idx++; // skip (
		node->left = buildParseTree(); // expression
		idx++; // skip )
	}
	else
	{
		node->left = createNode();
		node->left->value = input[idx++]; // operand
	}

	node->value = input[idx++]; // operator

	if(input[idx] == '(')
	{
		idx++; // skip (
		node->right = buildParseTree(); // expression
		idx++; // skip )
	}
	else
	{
		node->right = createNode();
		node->right->value = input[idx++]; // operand
	}

	return node;
}

void printParseTree(Node* node)
{
	if(node->left != NULL)
		printParseTree(node->left);
	if(node->right != NULL)
		printParseTree(node->right);
	printf("%c", node->value);
}

void freeParseTree(Node* node)
{
	if(node->left != NULL)
		freeParseTree(node->left);
	if(node->right != NULL)
		freeParseTree(node->right);
	free(node);
}

void processInput()
{
	idx = 1;
	Node* tree = buildParseTree();
	printParseTree(tree);
	printf("\n");
	freeParseTree(tree);
}

int main(int argc, char** argv)
{
	unsigned int numTests;
	scanf("%u\n", &numTests);

	unsigned int i;
	for(i = 0; i < numTests; i++)
	{
		scanf("%s", input);
		processInput();
	}

	return 0;
}

