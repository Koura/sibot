#pragma once
#include "QTable.h"
#include <ctime>
#include <fstream>
#include <cmath>

class Policy
{
public:
	Policy();
	~Policy();
	int chooseGreedyAction(int state, QTable* table);
	void saveConstants();
	int getEpisode();

private:
	bool loadConstants();
	
	int chooseRandom(int columns);
	bool is_empty(std::ifstream& file);

	int k;
	long double epsilon;
};