#pragma once
#include "QTable.h"
#include <ctime>
#include <fstream>
#include <cmath>
#include <BWAPI.h>

class Policy
{
public:
	Policy();
	~Policy();
	int chooseGreedyAction(int state, QTable* table, BWAPI::Unit* hero);
	void saveConstants();
	int getEpisode();

private:
	bool loadConstants();
	
	int chooseRandom(int columns, BWAPI::Unit* hero);
	bool is_empty(std::ifstream& file);

	int k;
	long double epsilon;
};