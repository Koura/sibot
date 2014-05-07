#include "Policy.h"

Policy::Policy()
{
	if(!loadConstants())
	{
		k = 1;
		epsilon = 0.9;
	}
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
	return action;
}

int Policy::chooseRandom(int columns)
{
	srand(time(0));
	return rand() % columns;
}

void Policy::saveConstants()
{
	k++;
	epsilon = ((double)epsilon*k)*((double)1/(k+1));
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