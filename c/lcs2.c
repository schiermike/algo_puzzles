/**
 * https://www.spoj.pl/problems/LCS2/
 * Longest common substring of lowercase letters
 * Schier Michael
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool char
#define false 0
#define true !false

#define MAXLEN 250000

char firstString[MAXLEN+1];
char otherString[MAXLEN+1];

#ifdef DEBUG
#include <time.h>
long count;
#endif

typedef struct __node
{
	// pointers to part-match in first string
	char* first;
	char* last;
	// up to 26 different suffixes possible
	struct __node* suffix[26];
	struct __node* parent;
} Node;

Node* newNode()
{
#ifdef DEBUG
	count++;
#endif
	Node* node = (Node*)malloc(sizeof(Node));
	node->first = NULL;
	node->last = NULL;
	node->parent = NULL;
	int i;
	for(i=0; i<26; i++)
		node->suffix[i] = NULL;
}

// recursively delete nodes and free memory
void deleteNode(Node* node)
{
	int i;
	for(i=0; i<26; i++)
		if(node->suffix[i] != NULL)
			deleteNode(node->suffix[i]);
			
#ifdef DEBUG
	count--;
#endif
	free(node);
}

void deleteChildrenExcept(Node* node, Node* child)
{
	int i;
	for(i=0; i<26; i++)
	{
		if(node->suffix[i] == NULL || node->suffix[i] == child)
			continue;
		deleteNode(node->suffix[i]);
		node->suffix[i] = NULL;
	}
}

void deleteChildren(Node* node)
{
	deleteChildrenExcept(node, NULL);
}

Node* buildSuffixTree(char* str)
{
	char* last = str + strlen(str);
	char* p;
	Node* root = newNode();
	root->first = root->last = str;
	int minMatchLen = 0;
	for(p = str; p != last; p++)
	{
		char* sfx = p;
		Node* node = root;
		while(true)
		{
			if(node != root)
			{
				// compare sequences and stop at a difference
				char* p3 = node->first;
				
				if(minMatchLen > 0) // make use of a known minMatchLen - no need for point-by-point checking
				{
					if(p3 + minMatchLen <= node->last) // some parts of this node match
					{
						p3 += minMatchLen;
						sfx += minMatchLen;
						minMatchLen = 0;
					}
					else // the whole node matches and also some of one node below
					{
						sfx += (node->last - p3);
						minMatchLen -= (node->last - p3);
						p3 = node->last;
					}
				}
				
				while(p3 < node->last && sfx < last && *p3 == *sfx)
				{
					p3++;
					sfx++;
				}
				if(sfx == last) // nothing to do left, this suffix is already included in the tree
					break;
				else if(p3 == node->last) // we have to continue the search at a lower node
				{
					// do nothing
				}
				else
				{
					// split this node because *p3 differs from *sfx
					// first clone this node
					Node* clone = newNode();
					clone->first = p3;
					clone->last = node->last;
					int i;
					for(i=0; i<26; i++)
					{
						clone->suffix[i] = node->suffix[i];
						if(clone->suffix[i] != NULL)
							clone->suffix[i]->parent = clone;
						node->suffix[i] = NULL;
					}
					
					// and create the new node
					Node* nNode = newNode();
					nNode->first = sfx;
					nNode->last = last;
						
					// then connect the new structure
					node->last = p3;
					node->suffix[*p3-97] = clone;
					node->suffix[*sfx-97] = nNode;
					
					clone->parent = node;
					nNode->parent = node;
					
					break;
				}
			}
		
			if(node->suffix[*sfx-97] == NULL) // no subnode exists, create a new one
			{
				Node* child = newNode();
				child->first = sfx;
				child->last = last;
				node->suffix[*sfx-97] = child;
				child->parent = node;
				break;
			}
			else // we already have a corresponding subnode, continue the search there!
			{
				node = node->suffix[*sfx-97];
			}
		}
		
		// speed up insert operations
		minMatchLen = p < sfx ? sfx - p - 1 : 0;
	}
	
	return root;
}

void intersectTrees(Node* root, char* p, Node* root2, char* p2)
{
	while(true)
	{
		if(p == root->last && p2 == root2->last)
		{
			 // intersect all possible subtrees
	 		int i;
			for(i=0; i<26; i++)
				if(root->suffix[i] != NULL && root2->suffix[i] != NULL)
					intersectTrees(root->suffix[i], root->suffix[i]->first, root2->suffix[i], root2->suffix[i]->first);
				else if(root->suffix[i] != NULL) // only tree 1 has child nodes -> we don't need them any more
				{
					deleteNode(root->suffix[i]);
					root->suffix[i] = NULL;
				}
			return;
		}
		else if(p == root->last) // move down in the first tree if possible
		{
			// we can forget about all other branches
			Node* child = root->suffix[*p2-97];
			root->suffix[*p2-97] = NULL;
			deleteChildrenExcept(root, child);
			if(child == NULL)
				return;
				
			if(root->parent != NULL) // merge this lonely node with its only child node
			{
				int strlen = root->last - root->first;
				// prepend string of this node
				child->first -= strlen; 
				// relink children
				root->parent->suffix[*(child->first)-97] = child;
				child->parent = root->parent;
				// finally get rid of this node
				deleteNode(root);
			}
				
			root = child;
			p = root->first;
		}
		else if (p2 == root2->last) // move down in the second tree if possible
		{
			if(root2->suffix[*p-97] == NULL)
			{
				// no further going down in tree 2 -> also forget about child nodes of tree 1
				deleteChildren(root);
				root->last = p;
				return;
			}
			root2 = root2->suffix[*p-97];
			p2 = root2->first;
		}
		else if(*p != *p2) // end of branch matching
		{
			deleteChildren(root);
			root->last = p;
			return;
		}
		else // match strings of local nodes
		{
			p++;
			p2++;
		}
	}
}

long calcLongestMatch(Node* node)
{
	long maxLen = 0;
	int i;
	for(i=0; i<26; i++)
		if(node->suffix[i] != NULL)
		{
			long tmp = calcLongestMatch(node->suffix[i]);
			if(tmp > maxLen)
				maxLen = tmp;
		}
	
	return (node->last - node->first) + maxLen;
}

int main(int argc, char** args)
{
#ifdef DEBUG
	clock_t start = clock();
#endif

	scanf("%s\n", firstString);
	
	Node* root = buildSuffixTree(firstString);

	while(scanf("%s\n", otherString) == 1)
	{
#ifdef DEBUG
		printf("creating otherTree - #nodes: %ld\n", count);
#endif

		Node* otherRoot = buildSuffixTree(otherString);
		
#ifdef DEBUG
		printf("intersecting trees - #nodes: %ld\n", count);
#endif

		intersectTrees(root, firstString, otherRoot, otherString);
		
#ifdef DEBUG
		printf("deleting unused nodes - #nodes: %ld\n", count);
#endif

		deleteNode(otherRoot);
		
#ifdef DEBUG
		printf("round done - #nodes: %ld\n", count);
#endif
	}

	printf("%ld\n", calcLongestMatch(root));
	deleteNode(root);

#ifdef DEBUG
	clock_t end = clock();
	double cpu_time = ((double)( end - start )) / CLOCKS_PER_SEC;
	printf("%.3fsecs\n", cpu_time);
#endif

	return 0;
}
