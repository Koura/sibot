#pragma once
#include <BWAPI.h>
#include <vector>
#include <sstream>
#include <limits>

class StateMastermind
{
public:
	StateMastermind();
	~StateMastermind();
	void create(int enemyCount);
	void destroy();
	int observe(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies);
	void setPrevious(int value);
	int getPrevious();
	void setPreviousA(int a);
	int getPreviousA();
	int getEnemyHP();
	void setAlliedHP(int value);
	int getAlliedHP();
	int getStateSize();

private:
	int weaponCooldown(BWAPI::Unit* hero);
	int distanceToClosestEnemy(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies);
	int enemiesInRange(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies);
	int currentHealth(BWAPI::Unit *hero);
	std::vector<std::string> m_states;
	int m_previousState;
	int m_previousAction;
	int m_hpEnemies;
	int m_hpAllies;
};