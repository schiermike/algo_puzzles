#include <stdio.h>
#include <stdlib.h>

void draw(int size)
{
		int line;
	int height = size * 4 - 1;
	for(line = 0; line < height; line++)
	{
		int i;

		int indent;
		if(line < height/2)
			indent = height/2 - 2*line;
		else
			indent = height/2 + 1 - (height-line)*2;
		
		if(indent > 0)
		{
			for(i = 0; i < indent; i++)
				printf(" ");

			if(line < height/2)
				printf("_");
			else printf("\\_/");
		}

		if(1 <= line && line < size)
		{
			i = line;
			while(i-->0)
				printf("/ \\_");
		}
			
		if( height - size < line && line < height)
		{
			i = height-line-1;
			while(i-->0)
				printf(" \\_/");
		}

		if( size <= line && line <= height - size)
		{
			int i = size;
			if(line%2 == 0 ^ size%2 == 0)
				while(i-->0)
				{
					if(i == 0)
						printf("\\_/");
					else
						printf("\\_/ ");
				}
			else
				while(i-->0)
				{
					if(i == 0)
						printf("/ \\");
					else
						printf("/ \\_");
				}
		}

		printf("\n");
	}
	printf("***\n");
}

int main(int argc, char** args)
{
	int size;
	while(1)
	{
		scanf("%d\n", &size);
		if(size == -1)
			break;
		draw(size);
	}
	
	return 0;
}

