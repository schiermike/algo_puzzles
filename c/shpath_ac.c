#include <stdio.h>
#include <string.h>

typedef struct
{
	char name[12];
	int firstroad;
} t_city;

int cities;
t_city city[11111];

int find_city(char name[])
{
	int i;
	
	for(i =1; i<=cities; i++)
		if(!strcmp(name, city[i].name))
			return i;	
	return 0;
}

typedef struct
{
	int from;
	int to;
	int len;
	int nextroad;
} t_road;

int roads;
t_road road[1111111];

void read_graph()
{
	int c,r;
	int to,len;
	scanf("%d", &cities);
	roads = 0;
	for(c=1; c <= cities; c++)
	{
		scanf("%s", city[c].name);
		city[c].firstroad = 0;
		scanf("%d", &r);
		while(r--)
		{
			scanf("%d%d", &to, &len);
			road[++roads].from = c;
			road[roads].to = to;
			road[roads].len = len;
			road[roads].nextroad = city[c].firstroad;
			city[c].firstroad = roads;
		}
	}
}

void read_query(int* from, int* to)
{
	char name[12];
	scanf("%s", name);
	*from = find_city(name);
	scanf("%s", name);
	*to = find_city(name);
}

#define INFINITY 2111111111

int calc_mindist(int from, int to)
{
	static int dist[11111], heap[11111], heappos[11111];
	int inheap;
	int c,r;
	int best;
	int p1, p2, p3, d1, d2, d3, t;
	
	for(c=1; c <= cities; c++)
	{
		dist[c] = INFINITY;
		heappos[c] = 0;
	}
	dist[from] = 0;
	heap[1] = from;
	heappos[from] = 1;
	inheap = 1;
	
	while(inheap)
	{
		best = heap[1];
		if(best == to)
			return dist[best];
		heappos[best] = 0;
		heap[1] = heap[inheap--];
		heappos[heap[1]] = 1;
		p1 = 1;
		while(1)
		{
			if(2*p1 > inheap)
				break;
			d1 = dist[heap[p1]];
			p2 = 2*p1;
			d2 = dist[heap[p2]];
			p3 = 2*p1+1;
			d3 = p3 > inheap ? INFINITY : dist[heap[p3]];
			if( d3 < d2 && d3 < d1 )
			{
				t = heap[p1];
				heap[p1] = heap[p3];
				heap[p3] = t;
				heappos[heap[p1]] = p1;
				heappos[heap[p3]] = p3;
				p1 = p3;
			}
			else if(d2 <= d3 && d2 <= d1)
			{
				t = heap[p1];
				heap[p1] = heap[p2];
				heap[p2] = t;
				heappos[heap[p1]] = p1;
				heappos[heap[p2]] = p2;
				p1 = p2;
			}
			else
				break;
		}
		
		for(r=city[best].firstroad; r>0; r=road[r].nextroad)
		{
			c = road[r].to;
			if(dist[best]+road[r].len >= dist[c])
				continue;
			dist[c] = dist[best]+road[r].len;
			if(!heappos[c])
			{
				heap[++inheap] = c;
				p1 = heappos[c] = inheap;
			}
			else
				p1 = heappos[c];
				
			while(p1 > 1)
			{
				d1 = dist[heap[p1]];
				p2 = p1/2;
				d2 = dist[heap[p2]];
				if(d2 <= d1)
					break;
				t = heap[p1];
				heap[p1] = heap[p2];
				heap[p2] = t;
				heappos[heap[p1]] = p1;
				heappos[heap[p2]] = p2;
				p1 = p2;
			}
		}
	}
	return -1;
}

int main()
{
	int cases, queries;
	int from, to;
	
	scanf("%d", &cases);
	while(cases--)
	{
		read_graph();
		scanf("%d", &queries);
		while(queries--)
		{
			read_query(&from, &to);
			printf("%d\n", calc_mindist(from, to));
		}
	}
	return 0;
}	
