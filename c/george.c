/**
 * https://www.spoj.pl/problems/GEORGE/
 * Schier Michael
 */

#include <stdio.h>
#include <stdlib.h>
#define bool char
#define false 0
#define true !false

typedef struct __period
{
	int start;
	int end;
	struct __period* next;
} BlockPeriod;

struct __road; // forward declaration of Road
typedef struct __road_entry
{
	struct __road* road;
	struct __road_entry* next;
} RoadEntry;

typedef struct
{
	int id;
	struct __road_entry* first;
	struct __road_entry* last;
} Cross;

typedef struct __road
{
	Cross* endA;
	Cross* endB;
	int cost;
	BlockPeriod* block;
} Road;


int n, m;
int a, b, k, g;
int* routeG;
Cross* cross;


int solve()
{
	int* earliest = (int*)malloc(n*sizeof(int));
	int i;
	for(i=0; i<n; i++)
		earliest[i] = 1000000000;
	earliest[a] = k;
	
	Cross** queue = (Cross**)malloc(n*sizeof(Cross*));
	for(i=0; i<n; i++)
		queue[i] = NULL;
	int qFirst = 0;
	queue[qFirst] = &cross[a];
	int qNextFree = 1;
	
	// visit all nodes until queue is empty
	while( queue[qFirst] != NULL )
	{
		// dequeue first entry
		Cross* c = queue[qFirst];
		queue[qFirst] = NULL;
		qFirst = (qFirst+1)%n;
		
		// check neighbors
		RoadEntry* re = c->first;
		while(re != NULL)
		{
			Cross* c2 = re->road->endA == c ? re->road->endB : re->road->endA;
			
			int early = earliest[c->id];
			// is this road blocked?
			BlockPeriod* pb = re->road->block;
			while(pb != NULL)
			{
				if(pb->start <= early && early <= pb->end)
					early = pb->end+1;
				pb = pb->next;
			}
			
			early += re->road->cost;
			
			if(early < earliest[c2->id])
			{
				earliest[c2->id] = early;
				// have we already enqueued this node?
				bool inQueue = false;
				for(i=qFirst; queue[i]!=NULL; i = (i+1)%n)
					if(queue[i] == c2)
					{
						inQueue = true;
						break;
					}
				if(!inQueue)
				{
					queue[qNextFree] = c2;
					qNextFree = (qNextFree+1)%n;
				}
			}
			re = re->next;
		}
	}
	
	
	return earliest[b]-k;
}

void initBlock()
{
	int i;
	int timeGeorge = 0;
	for(i=0; i<g-1; i++)
	{
		Cross* c1 = &cross[routeG[i]];
		Cross* c2 = &cross[routeG[i+1]];
	
		RoadEntry* re = c1->first;
		while(true)
		{
			Road* r = re->road;
			if(r->endA == c2 || r->endB == c2)
			{
				BlockPeriod* bp = (BlockPeriod*)malloc(sizeof(BlockPeriod));
				bp->start = timeGeorge;
				timeGeorge += r->cost;
				bp->end = timeGeorge-1;
				bp->next = NULL;
				
				
				if(r->block == NULL)
					r->block = bp;
				else
				{
					BlockPeriod* predecessor = r->block;
					while(predecessor->next != NULL)
						predecessor = predecessor->next;
					predecessor->next = bp;
				}
				break;
			}
			re = re->next;
		}
	}
}

int main()
{
	scanf("%d %d\n", &n, &m);
	scanf("%d %d %d %d\n", &a, &b, &k, &g);
	a--;
	b--;
	
	routeG = (int*)malloc(g*sizeof(int));
	int i;
	for(i=0; i<g; i++)
	{
		scanf("%d", &routeG[i]);
		routeG[i]--;
	}
	
	cross = (Cross*)malloc(n*sizeof(Cross));
	for(i=0; i<n; i++)
	{
		cross[i].id = i;
		cross[i].first = cross[i].last = NULL;
	}
	
	int from, to, cost;
	// build data structure for road grid
	for(i=0; i<m; i++)
	{
		scanf("%d %d %d\n", &from, &to, &cost);
		from--;
		to--;
		
		Road* road = (Road*)malloc(sizeof(Road));
		road->endA = &cross[from];
		road->endB = &cross[to];
		road->cost = cost;
		road->block = NULL;
		
		RoadEntry* entry = (RoadEntry*)malloc(sizeof(RoadEntry));
		entry->road = road;
		if(cross[from].first == NULL)
			cross[from].first = entry;
		else
			cross[from].last->next = entry;
		cross[from].last = entry;
		
		entry = (RoadEntry*)malloc(sizeof(RoadEntry));
		entry->road = road;
		if(cross[to].first == NULL)
			cross[to].first = entry;
		else
			cross[to].last->next = entry;
		cross[to].last = entry;
	}
	
	initBlock();
	
	printf("%d\n", solve());
	return 0;
}

