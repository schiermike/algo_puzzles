#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define min(x, y)  ( (x) < (y) ? (x) : (y) )

double 
timer (void)
{
  struct rusage r;

  getrusage(0, &r);
  return (double) (r.ru_utime.tv_sec + r.ru_utime.tv_usec / (double)1000000);
}

struct v2node;

typedef struct v1node
{
	int number;
	int numOut;
	int label;
	int nextScan;
	struct v2node **out;
	struct v2node *parent;
	int lastRelabel;
	struct v2node *backtrack;
	struct v1node *next;
} V1Node;

typedef struct v2node
{
	int number;
	int numIn;
	int label;
	V1Node **in;
	V1Node *equalChild;
	V1Node *greaterChild;
	struct v2node *next;
	int visited;
	int topScan;
} V2Node;

static V1Node *left = NULL;
static V2Node *right = NULL;
static int n1 = 0;
static int n2 = 0;
static int m = 0;
static V2Node **distance = NULL;

static int highestWT2=1;
static char errMsg [1024];
static int numRelabels = 0;
static int matching=0;

static void
createList (void) 
{
	int lineLength=1024, *from=NULL, *to=NULL, i;

	char *line;

	if ((line = (char *) malloc ((lineLength+1) * sizeof (char))) == NULL)
	{
		printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
		exit (1);
	}

	fgets (line, lineLength, stdin);

sscanf (line, "%d %d %d", &n1, &n2, &m);

			if ((left = (V1Node *) malloc (n1 * sizeof (V1Node))) == NULL)
			{
				printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
				exit (1);
			}

			if ((distance = (V2Node **) malloc (n1 * sizeof (V2Node *))) == NULL)
			{
				printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
				exit (1);
			}

			if ((right = (V2Node *) malloc (n2 * sizeof (V2Node))) == NULL)
			{
				printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
				exit (1);
			}

			if ((from = (int *) malloc (m * sizeof (int))) == NULL)
			{
				printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
				exit (1);
			}

			if ((to = (int *) malloc (m * sizeof (int))) == NULL)
			{
				printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
				exit (1);
			}

			for (i=0; i<n1; ++i)
			{
				distance[i] = NULL;
				left[i].numOut = 0;
			}
			for (i=0; i<n2; ++i)
			{
				right[i].numIn = 0;
			}
			m = 0;

	while (fgets (line, lineLength, stdin))
	{
			sscanf (line, "%d %d", &from[m], &to[m]);
			from[m] --;
			to[m] --;
			++ left[from[m]].numOut;
			++ right[to[m]].numIn;
			++ m;
	}
	
	for (i=0; i<n1; ++i)
	{
		if ((left[i].out = (V2Node **) malloc (left[i].numOut * sizeof (V2Node *))) == NULL)
		{
			printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
			exit (1);
		}

		left[i].nextScan = left[i].numOut;
		left[i].numOut = 0;
		left[i].number = (i+1);
		left[i].parent = NULL;
		left[i].next = NULL;
		left[i].label = n1;
		left[i].lastRelabel = 0;
		left[i].backtrack = NULL;
	}

	for (i=0; i<n2; ++i)
	{
		if ((right[i].in = (V1Node **) malloc (right[i].numIn * sizeof (V1Node *))) == NULL)
		{
			printf ("%s, %d: Could not allocate memory.\n", __FILE__, __LINE__);
			exit (1);
		}

		right[i].label = 0;
		right[i].numIn = 0;
		right[i].number = (i+1);
		right[i].equalChild = NULL;
		right[i].greaterChild = NULL;
		right[i].visited = 0;
		right[i].topScan = 0;
	}


	for (i=m; i--; )
	{
		left[from[i]].out[left[from[i]].numOut] = &right[to[i]];
		++ left[from[i]].numOut;
		right[to[i]].in[right[to[i]].numIn] = &left[from[i]];
		++ right[to[i]].numIn;
	}

	free (from);
	from = NULL;

	free (to);
	to = NULL;

	free (line);
	line = NULL;
}

static void
greedyInitialize (void)
{
	int i, j;
	V1Node *leftNode;
	
	for (i=0; i<n1; ++i)
	{
		leftNode = &left[i];

		for (j=0; j<leftNode->numOut; ++j)
		{
			if (leftNode->out[j]->equalChild == NULL)
			{
				leftNode->parent = leftNode->out[j];
				leftNode->parent->equalChild = leftNode;
				leftNode->backtrack = leftNode->parent;
				leftNode->label = 1;
				++ matching;
				leftNode->nextScan = (j+1);
				break;
			}
		}
	}

	for (i=0; i<n2; ++i)
	{
		if (right[i].equalChild)
		{
			right[i].label = 1;
			right[i].next = distance[1];
			distance[1] = &right[i];
		}
	}
}


static void
generateOneLayer (void)
{
	int j;

	V1Node *tempChild=NULL;

	V2Node *last=NULL, *current=NULL;

	last = distance[1];

	for ((current = last->next); (current); (current=current->next))
	{
		tempChild = current->equalChild;

		if (tempChild->label == 2)
		{
			last->next = current->next;
			current = last;
			continue;
		}

		for (j=tempChild->nextScan; j < tempChild->numOut; ++ j)
		{
			if (tempChild->out[j]->label == 0)
			{
				tempChild->nextScan = j;
				tempChild->lastRelabel = numRelabels;
				goto END_FOR_A;
			}
		}

		tempChild->nextScan = j;
		last->next = current->next;
		current = last;		

		//////////
		END_FOR_A:
		//////////

		last = current;
	}

	current = distance[1];
	tempChild = current->equalChild;

	if (tempChild->label == 2)
	{
		distance[1] = current->next;
		return;
	}

	for (j=tempChild->nextScan; j < tempChild->numOut; ++ j)
	{
		if (tempChild->out[j]->label == 0)
		{
			tempChild->nextScan = j;
			tempChild->lastRelabel = numRelabels;
			return;
		}
	}
	tempChild->nextScan = j;

	distance[1] = current->next;	
}



static int
globalRelabel (void)
{
	int i, j;

	V1Node *temp=NULL;

	V2Node *current=NULL;

	++ numRelabels;

	generateOneLayer ();
	
	if (distance[1] == NULL)
	{
		return 0;
	}

	for (i=1; (i < n1) && (distance[i]); ++ i)
	{
		distance[i+1] = NULL;

		for (current = distance[i]; (current); current=current->next)
		{
			current->visited = 0;

			for (j=0; j<current->numIn; ++j)
			{
				temp = current->in[j];

				if (temp->lastRelabel < numRelabels)
				{
					temp->lastRelabel = numRelabels;

					temp->label = (i+1);
					temp->nextScan = 0;

					if (temp->parent)
					{
						temp->parent->label = (i+1);
						temp->parent->next = distance[i+1];
						distance[i+1] = temp->parent;
					}
					else
					{
						current->topScan = j;
						highestWT2 = i;
						return 1;
					}
				}
			}
		}
	}

	return 0;
}


static int
findMergers (V2Node *root)
{
	V1Node *newChild;
	V2Node *oldRoot, *newRoot;
	int j, k, ellMinus;

	root->visited = 1;

	for (j=root->topScan; j<root->numIn; ++j)
	{
		if ((root->in[j])->parent == NULL)
		{
			root->topScan = j;
			newChild = root->in[j];

			newChild->label = (root->label+1);
			newChild->parent = root;
			root->greaterChild = newChild;
			newChild = root->equalChild;

			while (newChild->parent)
			{
				////////
				TOP_FOR:
				////////

				ellMinus = (newChild->label-1);

				for (k=newChild->nextScan; k<newChild->numOut; ++k)
				{
					if (((newChild->out[k])->visited == 0) && 
						((newChild->out[k])->label == ellMinus))
					{
						newRoot = newChild->out[k];
						newChild->nextScan = k;
						newRoot->visited = 1;

						if (newRoot->label == 0) // augmentation
						{
							++ matching;

							oldRoot = newChild->backtrack;
							newRoot->equalChild = newChild;
							newChild->parent = newRoot;
							newChild->backtrack = newRoot;
							newRoot->label = 1;
							newRoot->next = distance[1];
							distance[1] = newRoot;

							for ((newRoot=oldRoot); (newRoot); (newRoot=oldRoot))
							{
								newChild = oldRoot->greaterChild;
								oldRoot = newChild->backtrack;
								newRoot->equalChild = newChild;
								newChild->backtrack = newRoot;
								newRoot = oldRoot;
							}

							return 1;
						}

						newRoot->greaterChild = newChild;
						newChild->parent = newRoot;
						newChild = newRoot->equalChild;
						
						goto TOP_FOR;
					}
				}
				newChild->nextScan = k;
				
				newChild = newChild->parent->greaterChild;
				newChild->parent = newChild->backtrack;
			}
			return 0;
		}
	}

	root->topScan = j;

	return 0;
}


static int
solve (void)
{
	V2Node *current;

	while (globalRelabel ())
	{
		for (current=distance[highestWT2]; (current); current=current->next)
		{
			findMergers (current);
		}
	}

	return 0;
}


static int
freeMemory (void)
{
	int i;
	for (i=0; i<n1; i++)
	{
		free (left[i].out);
		left[i].out = NULL;
	}
	free (left);
	left = NULL;

	for (i=0; i<n2; i++)
	{
		free (right[i].in);
		right[i].in = NULL;
	}

	free (right);
	right = NULL;

	free(distance);
	distance = NULL;

	return 0;
}

static int
checkFeasOpt (void)
{
	int i, flow=0, cut=0;
	
	for (i=0; i<n2; i++)
	{
		if (right[i].equalChild)
		{
			++ flow;
			if (right[i].equalChild->lastRelabel < numRelabels)
			{
				++ cut;
			}
		}
	}

	for (i=0; i<n1; i++)
	{
		if (left[i].lastRelabel == numRelabels)
		{
			++ cut;
		}
	}

	if (cut != flow)
	{
		sprintf (errMsg, "Error: Max flow (%d) not equal to min cut (%d)", flow, cut);
		return 0;
	}

	sprintf (errMsg, "Solution checks as feasible and optimal");

	return flow;
}

int 
main () {

	double startRead, endRead, endSolve;

	startRead = timer();

	createList ();

	endRead = timer ();

	greedyInitialize ();

	solve ();

	endSolve = timer ();

	printf ("%d\n", checkFeasOpt ());

	freeMemory ();

	return 0;
}


