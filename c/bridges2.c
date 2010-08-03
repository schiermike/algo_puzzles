/**
 * https://www.spoj.pl/problems/BRIDGES2/
 * Schier Michael
 */

#include <stdio.h>
#define bool char
#define false 0
#define true !false

typedef struct
{
	int c;
	int t;
	int cOnBridge;
	int tOnBridge;
	int cQueued;
} Bridge;

Bridge bridge[20];

int b;


int simulate()
{
	int time = 0;
	
	while(true)
	{
		int i;
		bool finished = true;
		for(i=0; i<b; i++)
		{
			// passengers passed bridge
			if(bridge[i].tOnBridge == bridge[i].t)
			{
				bridge[i].tOnBridge = 0;
				// enqueue at next bridge, if this one is not the last one
				if(i+1<b)
					bridge[i+1].cQueued += bridge[i].cOnBridge;
				bridge[i].cOnBridge = 0;
			}
			
			// new passengers can enter the bridge
			if(bridge[i].cOnBridge == 0 && bridge[i].cQueued != 0)
			{
				if(bridge[i].cQueued > bridge[i].c)
					bridge[i].cOnBridge = bridge[i].c;
				else
					bridge[i].cOnBridge = bridge[i].cQueued;
					
				bridge[i].cQueued -= bridge[i].cOnBridge;
			}
		
			// passengers move a bit
			if(bridge[i].cOnBridge != 0)
				bridge[i].tOnBridge++;
				

			finished &= bridge[i].cQueued == 0 && bridge[i].cOnBridge == 0;
		}
		
		if(finished)
			return time;
		
		time++;
	}
}

int main()
{
	while(1)
	{
		scanf("%d %d\n", &b, &bridge[0].cQueued);
		if(b==0)
			break;
		b = -b;
		
		int i;
		for(i=0; i<b; i++)
			scanf("%d %d\n", &bridge[i].c, &bridge[i].t);
		
		printf("%d\n", simulate());
	
	}

	return 0;
}

