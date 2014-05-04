#include "Policy.h"

Policy::Policy()
{
	k = 1;
	epsilon = 0.9;
}

Policy::~Policy()
{
}

int Policy::chooseGreedyAction(int state, QTable* table)
{
	int action = 0;
	srand(time(0));
	double p = rand()/double(RAND_MAX);
	if(p < epsilon)
	{
		action = chooseRandom(table->getColumns());
	}
	else
	{
		action = table->maxAction(state);
	}
	k++;
	epsilon *= 1/k;
	return action;
}

int Policy::chooseRandom(int columns)
{
	srand(time(0));
	return rand() % columns;
}