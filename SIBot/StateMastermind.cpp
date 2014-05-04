#include "StateMastermind.h"
#include "Action.h"
#include <algorithm>
#include <limits>

StateMastermind::StateMastermind()
{
}

StateMastermind::~StateMastermind()
{
}
void StateMastermind::create(int enemyCount)
{
	m_previousState = -1;
	std::ostringstream s;
	std::string state;
	for(int i = 0; i <= 1; i++)
	{
		for(int j = 1; j <= 4; j++)
		{
			for(int k = 0; k <= enemyCount; k++)
			{
				for(int l = 1; l <=4; l++)
				{
					s << i << j <<k << l;
					state.append(s.str());
					m_states.push_back(state);
					s.str(std::string());
					state.clear();
				}
			}
		}
	}
}

void StateMastermind::destroy()
{
	m_states.clear();
}

int StateMastermind::observe(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies)
{
	std::stringstream sstr;
	sstr << weaponCooldown(hero);
	sstr << distanceToClosestEnemy(hero, enemies);;
	sstr << enemiesInRange(hero, enemies);
	sstr << currentHealth(hero);
    std::string str1 = sstr.str();
	return std::find(m_states.begin(), m_states.end(), str1) - m_states.begin();
}

int StateMastermind::distanceToClosestEnemy(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies)
{
	int x1 = hero->getPosition().x();
	int y1 = hero->getPosition().y();
	int closest = std::numeric_limits<int>::max();
	int dist = 0;
	for(std::set<BWAPI::Unit*>::iterator it = enemies.begin(); it!=enemies.end(); ++it)
	{ 
		BWAPI::Position enePos = (*it)->getPosition();
	    dist = Action::distance(x1, y1, enePos);
		if(closest > dist)
		{
			closest = dist;
		}
	}
	if(closest <= 45)
	{
		return 1;
	}
	if(closest <= 90)
	{
		return 2;
	}
	if(closest <= 135)
	{
		return 3;
	}
	return 4;
}
int StateMastermind::weaponCooldown(BWAPI::Unit *hero)
{
	if(hero->getGroundWeaponCooldown() == 0 && hero->getAirWeaponCooldown() == 0)
	{
		return  0;
	}
	return 1;
}

int StateMastermind::enemiesInRange(BWAPI::Unit *hero, std::set<BWAPI::Unit*> enemies)
{
	int n = 0;
	m_hpEnemies = 0;
	for(std::set<BWAPI::Unit*>::iterator it = enemies.begin(); it!=enemies.end(); ++it)
	{ 
		m_hpEnemies += (*it)->getHitPoints();
		if(hero->isInWeaponRange(*it))
		{
			n++;
		}
	}
	return n;
}

int StateMastermind::currentHealth(BWAPI::Unit *hero)
{
	double relativeHp = (double)hero->getHitPoints() / hero->getType().maxHitPoints();
	if(relativeHp <= 0.25)
	{
		return 1;
	}
	if(relativeHp <=0.50)
	{
		return 2;
	}
	if(relativeHp <= 0.75)
	{
		return 3;
	}
	return 4;
}

void StateMastermind::setPrevious(int value)
{
	m_previousState = value;
}

int StateMastermind::getPrevious()
{
	return m_previousState;
}

void StateMastermind::setPreviousA(int a)
{
	m_previousAction = a;
}

int StateMastermind::getPreviousA()
{
	return m_previousAction;
}

int StateMastermind::getEnemyHP()
{
	return m_hpEnemies;
}

int StateMastermind::getAlliedHP()
{
	return m_hpAllies;
}

void StateMastermind::setAlliedHP(int value)
{
	m_hpAllies = value;
}
