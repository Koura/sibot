#include "Action.h"
#include <cmath>

void Action::fight(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies)
{
	BWAPI::Unit* target = 0;
	int x1 = hero->getPosition().x();
	int y1 = hero->getPosition().y();
	float closest = std::numeric_limits<float>::max();
	float dist = 0;
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
		else
		{
			if(target==0)
			{
			dist = distance(x1, y1, (*it)->getPosition());
			if(closest > dist)
				{
					closest = dist;
					target = *it;
				}
			}
		}
	}
	if(target!=0 && !hero->isAttackFrame() && !hero->isStartingAttack() && !hero->isAttacking())
	{
		hero->attack(target, false);
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

void Action::useAbility(BWAPI::Unit* hero)
{
	hero->useTech(BWAPI::TechTypes::Stim_Packs);
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