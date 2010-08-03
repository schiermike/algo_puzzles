/**
 * https://www.spoj.pl/problems/SPLIT/
 * Schier Michael
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct
{
	int w;
	int h;
} Dim;

typedef struct __node
{
	char c;
	Dim min;
	Dim dim;
	struct __node* left;
	struct __node* right;
} Node;

Node root;

void calcDim(Node* node)
{
	switch(node->c)
	{
		case '|': // enlarge left and right subwindow, round up left and round down bottom window
			node->left->dim.h = node->right->dim.h = node->dim.h;
			node->left->dim.w = node->left->min.w * node->dim.w / node->min.w + ((node->left->min.w * node->dim.w) % node->min.w > 0 ? 1 : 0);
			node->right->dim.w = node->right->min.w * node->dim.w / node->min.w;
			break;
		case '-': // enlarge top and bottom subwindow, round up top and round down bottom window
			node->left->dim.w = node->right->dim.w = node->dim.w;
			node->left->dim.h = node->left->min.h * node->dim.h / node->min.h + ((node->left->min.h * node->dim.h) % node->min.h > 0 ? 1 : 0);
			node->right->dim.h = node->right->min.h * node->dim.h / node->min.h;
			break;
		default:
			return; // do not process leave nodes
	}
	
	calcDim(node->left);
	calcDim(node->right);
}

void calcMinDim(Node* node)
{
	if(node->left == NULL)
	{
		// minimum dimension of a leave node
		node->min.w = 2;
		node->min.h = 2;
		return;
	}
	
	calcMinDim(node->left);
	calcMinDim(node->right);
	
	switch(node->c)
	{
		case '-':
			node->min.w = MAX(node->left->min.w, node->right->min.w);
			node->min.h = node->left->min.h + node->right->min.h;
			break;
		case '|':
			node->min.w = node->left->min.w + node->right->min.w;
			node->min.h = MAX(node->left->min.h, node->right->min.h);
			break;
	}
}

void buildTree(Node* node, char** c)
{
	node->c = **c;
	(*c)++;
	if(node->c == '|' || node->c == '-')
	{
		node->left = (Node*)malloc(sizeof(Node));
		buildTree(node->left, c);
		node->right = (Node*)malloc(sizeof(Node));
		buildTree(node->right, c);		
	}
	else
	{
		node->left = NULL;
		node->right = NULL;
	}
}

void freeTree(Node* node)
{
	if(node->left != NULL)
	{
		freeTree(node->left);
		free(node->left);
	}
	if(node->right != NULL)
	{
		freeTree(node->right);
		free(node->right);
	}
}

void drawWindow(Node* node, char** out, int wOff, int hOff)
{
	int i;
	for(i = wOff+1; i < wOff + node->dim.w; i++)
	{
		if(out[hOff][i] == ' ') out[hOff][i] = '-';
		if(out[hOff+node->dim.h][i] == ' ') out[hOff+node->dim.h][i] = '-';
	}
	for(i = hOff+1; i < hOff + node->dim.h; i++)
	{
		if(out[i][wOff] == ' ') out[i][wOff] = '|';
		if(out[i][wOff+node->dim.w] == ' ') out[i][wOff+node->dim.w] = '|';
	}
	out[hOff][wOff] = '*';
	out[hOff+node->dim.h][wOff] = '*';
	out[hOff][wOff+node->dim.w] = '*';
	out[hOff+node->dim.h][wOff+node->dim.w] = '*';
	
	
	if(node->c == '|')
	{
		drawWindow(node->left, out, wOff, hOff);
		drawWindow(node->right, out, wOff + node->left->dim.w, hOff);
	}
	else if(node->c == '-')
	{
		drawWindow(node->left, out, wOff, hOff);
		drawWindow(node->right, out, wOff, hOff + node->left->dim.h);
	}
	else
		out[hOff][wOff] = node->c;		
}

void printWindows()
{
	char** out = (char**)malloc( (root.dim.h+1) * sizeof(char*) );
	int i,j;
	for(i=0; i<root.dim.h+1; i++)
	{
		out[i] = (char*)malloc( (root.dim.w+2) * sizeof(char) );
		for(j=0; j<root.dim.w+1; j++)
			out[i][j] = ' ';
		out[i][root.dim.w+1] = '\0';
	}
	
	drawWindow(&root, out, 0, 0);
	
	for(i=0; i<root.dim.h+1; i++)
	{
		printf("%s\n", out[i]);
		free(out[i]);
	}
	free(out);
}

int main()
{
	int t;
	scanf("%d\n", &t);
	char input[200];
	int i;
	for(i=1; i<=t; i++)
	{
		scanf("%s\n", input);
		printf("%d\n", i);
		
		char* c = input;
		buildTree(&root, &c);
		calcMinDim(&root);
		root.dim = root.min; // the outer window is the limiting boundary
		calcDim(&root);
		
		printWindows();
		
		freeTree(&root);
	}

	return 0;
}

