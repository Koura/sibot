#include "Action.h"
#include <cmath>

void Action::fight(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies)
{
	BWAPI::Unit* target = 0;

	for(std::set<BWAPI::Unit*>::iterator it = enemies.begin(); it!=enemies.end(); ++it)
	{ 
		if(hero->isInWeaponRange(*it))
		{
			if(target!=0)
			{
				if(target->getHitPoints() > (*it)->getHitPoints())
				{
					target = *it;
				}
			}
			else 
			{
				target = *it;
			}
		}
	}
	if(target!=0)
	{
		hero->rightClick(target, false);
	}
}

void Action::run(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies)
{
	BWAPI::Position newPosi = hero->getPosition();
	BWAPI::Position enePos = (*enemies.begin())->getPosition();
    float dist = Action::distance(newPosi, enePos);
	float dx = newPosi.x() - enePos.x();
	float dy = newPosi.y() - enePos.y();
    dx /= dist;
	dy /= dist;
    newPosi.x() = newPosi.x() + dx*30;
	newPosi.y() = newPosi.y() + dy*30;
	hero->move(newPosi, false);
}

float Action::distance(BWAPI::Position a, BWAPI::Position b)
{
	int dx = a.x() - b.x();
	int dy = a.y() - b.y();
	return sqrt((float(dx*dx + dy*dy)));
}

float Action::distance(int x, int y, BWAPI::Position b)
{
	int dx = x - b.x();
	int dy = y - b.y();
	return sqrt((float(dx*dx + dy*dy)));
}