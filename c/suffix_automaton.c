/**
* This program creates a deterministic finite suffix automaton from an input string containing lower-case letters
* -1 is the initial state
* all states containing the number len(input) are accepting states
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 250000

#define bool int
#define false 0
#define true (!false)

typedef struct _index
{
	int i;
	struct _index* next;
	struct _index* last;
} Index;

typedef struct _state
{
	struct _state* next[26]; // successor states of the DFA from the current state
	struct _state* nextInList;
	Index* index;
} State;

char string1[MAX_LEN+1];
int inputLength;

State* firstState = NULL;
State* lastState = NULL;

Index* newIndex(int i)
{
	Index* index = (Index*)malloc(sizeof(Index));
	index->i = i;
	index->next = NULL;
	index->last = index;
	return index;
}

State* newState(Index* index)
{
	State* s = (State*)malloc(sizeof(State));

	s->index = index;

	s->nextInList = NULL;

	int i;
	for(i = 0; i < 26; i++)
		s->next[i] = NULL;
	
	return s;
}

bool isEqual(Index* i1, Index* i2)
{
	while(i1 != NULL && i2 != NULL)
	{
		if(i1->i != i2->i)
			return false;
		i1 = i1->next;
		i2 = i2->next;
	}
	
	return (i1 == NULL) ^ (i2 != NULL);
}

State* createStateIfNonExisting(State* state, int index)
{
	State* s = firstState;
	State* newState = state->next[index];
	while(s != NULL)
	{
		if( isEqual(s->index, newState->index) )
		{
			// we already have such a state
			state->next[index] = s; // use the existing one
			free(newState); // and forget about the newly created one
			return;
		}
		s = s->nextInList;
	}

	// this is a new state, let's add it to the state list
	lastState->nextInList = newState;
	lastState = lastState->nextInList;
}

void createStatesIfNonExisting(State* state)
{
	int i;
	for(i = 0; i < 26; i++)
	{
		if(state->next[i] == NULL)
			continue;
		createStateIfNonExisting(state, i);
	}
}

// create the next transition index after consuming the input character or return NULL if this is not a valid transition
State* createTransition(State* currentState, char input)
{
	Index* index = currentState->index;
	Index* firstIdx = NULL;
	while(index != NULL)
	{
		int i = index->i+1;
		if(i < inputLength && string1[i] == input)
		{
			Index* idx = newIndex(i);
			if(firstIdx == NULL)
			{
				firstIdx = idx;
				firstIdx->last = firstIdx;
			}
			else
			{
				firstIdx->last->next = idx;
				firstIdx->last = firstIdx->last->next;
			}
		}
		index = index->next;
	}

	if(firstIdx != NULL)
		return newState(firstIdx);
	return NULL;
}

#ifdef DEBUG
void printState(State* state)
{
	printf("{");
	Index* i = state->index;
	while(i != NULL)
	{
		if(state->index != i)
			printf(",");
		printf("%d", i->i);
		i = i->next;
	}
	printf("}");
}

void printTable()
{
	State* s = firstState;
	while(s != NULL)
	{
		printState(s);
		printf("     ");

			int i;
		for(i = 0; i < 26; i++)
		{
			if(s->next[i] == NULL)
				continue;
			printf("%c>", i+'a');
			printState(s->next[i]);
			printf(" ");
		}

		printf("\n");
		s = s->nextInList;
	}
}
#endif

int main(int argc, char** args)
{
	inputLength = 0;

	scanf("%s\n", string1);

	// create initial state pointing to the string index 0
	// create state table (starting with the initial state)
	firstState = lastState = newState(newIndex(-1));

	// create the initial states' next states
	while(string1[inputLength] != '\0' && inputLength < MAX_LEN)
	{
		Index* idx = newIndex(inputLength);
		int i = string1[inputLength] - 'a';
		if(firstState->next[i] == NULL)
		{
			firstState->next[i] = newState(idx);
		}
		else
		{
			firstState->next[i]->index->last->next = idx;
			firstState->next[i]->index->last = idx;
		}
		inputLength++;
	}

	createStatesIfNonExisting(firstState);

	// the initial state is fully specified (tons of stuff due to epsilon transitions), let's start with the second state
	State* s = firstState->nextInList;
	while(s != NULL)
	{
		int i;
		for(i = 0; i < 26; i++)
		{
			s->next[i] = createTransition(s, i+'a');
		}

		createStatesIfNonExisting(s);
		s = s->nextInList;
	}
#ifdef DEBUG
	printTable();
#endif

	return 0;
}
