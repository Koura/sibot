#pragma once
#include "QTable.h"
#include <ctime>

class Policy
{
public:
	Policy();
	~Policy();
	int chooseGreedyAction(int state, QTable* table);
private:

	int chooseRandom(int columns);

	int k;
	double epsilon;
};