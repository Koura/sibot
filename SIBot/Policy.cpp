#include "Policy.h"

Policy::Policy()
{
	srand(time(NULL));
	if(!loadConstants())
	{
		k = 1;
		epsilon = 0.9;
	}
}

Policy::~Policy()
{
}

int Policy::chooseGreedyAction(int state, QTable* table, BWAPI::Unit* hero)
{
	int action = 0;
	double p = rand()/double(RAND_MAX);
	if(p < epsilon)
	{
		action = chooseRandom(table->getColumns(), hero);
		//BWAPI::Broodwar->sendText("RESEARCH");
	}
	else
	{
		action = table->maxAction(state, (hero->getStimTimer()==0));
		//BWAPI::Broodwar->sendText("EXPLOIT");
	}
	return action;
}

int Policy::chooseRandom(int columns, BWAPI::Unit* hero)
{
	if(hero->getStimTimer()==0)
	{
		return rand() % columns;
	}
	return rand() % (columns-1);
}

void Policy::saveConstants()
{
	k++;
	epsilon = ((double)epsilon*(k-0.05))*((double)1/(k));
	std::ofstream file("constants.txt");
	if(file.is_open())
	{
		file << epsilon << "\n" << k;
		file.close();
	}
}

bool Policy::loadConstants()
{
	std::ifstream file("constants.txt");
	if(!is_empty(file))
	{
		if(file.is_open())
		{
			double d;
			file >> d;
			epsilon = d;
			file >> d;
			k = d;
			file.close();
			return true;
		}	
	}
	return false;
}

bool Policy::is_empty(std::ifstream& file)
{
	return file.peek() == std::ifstream::traits_type::eof();
}

int Policy::getEpisode()
{
	return k;
}