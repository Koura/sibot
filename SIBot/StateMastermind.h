#pragma once
#include <BWAPI.h>
#include <vector>
#include <sstream>

class StateMastermind
{
public:
	StateMastermind();
	~StateMastermind();
	void create(int enemyCount);
	void destroy();
	int observe(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies);
private:
	int weaponCooldown(BWAPI::Unit* hero);
	int distanceToClosestEnemy(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies);
	int enemiesInRange(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies);
	int currentHealth(BWAPI::Unit *hero);
	std::vector<std::string> m_states;
};