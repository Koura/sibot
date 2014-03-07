#include "SIBot.h"
#include "Action.h"
#include "../../Addons/Util.h"
#include "../../include/BWTA.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace BWAPI;

void SIBot::onStart()
{
	if(Broodwar->isReplay()) return;
	m_stateMaster = 0;
	Broodwar->enableFlag(Flag::UserInput);
    BWTA::readMap();
    BWTA::analyze();
	BWAPI::Race race = Broodwar->self()->getRace();
	BWAPI::Race enemyRace = Broodwar->enemy()->getRace();
	Broodwar->sendText("Spanish inquisition online");
	m_heroes = Broodwar->self()->getUnits();
	m_enemies = Broodwar->enemy()->getUnits();
	m_stateMaster = new StateMastermind();
	if(m_stateMaster!=0)
	{
		m_stateMaster->create(m_enemies.size());
	}
}

SIBot::~SIBot()
{
}

void SIBot::onEnd(bool isWinner)
{
	m_stateMaster->destroy();
	m_stateMaster = 0;
}

void SIBot::onFrame()
{
	if (Broodwar->isReplay()) return;
	for(std::set<Unit*>::iterator it = m_heroes.begin(); it!=m_heroes.end(); ++it)
	{
		if(!(*it)->isAttackFrame() && !(*it)->isMoving())
		{
			int currentState = m_stateMaster->observe((*it), m_enemies);
			std::stringstream sstr;
	        sstr << currentState;
            std::string str1 = sstr.str();
			Broodwar->sendText(str1.c_str());
			srand(time(NULL));
			if(rand()%2 == 0) 
			{
			//	Broodwar->sendText("FIGHT");
				Action::fight((*it), m_enemies);	
			}
			else 
			{
			//	Broodwar->sendText("RUN");
				Action::run((*it), m_enemies);
			}
		}
	}
}

void SIBot::onUnitDestroy(BWAPI::Unit* unit)
{
	if(Broodwar->isReplay()) return;
	if(Broodwar->self()->isEnemy(unit->getPlayer()))
	{
		m_enemies.erase(unit);
		return;
	}
	m_heroes.erase(unit);
}

void SIBot::onUnitDiscover(BWAPI::Unit* unit)
{
	if(Broodwar->isReplay()) return;
}

void SIBot::onUnitEvade(BWAPI::Unit* unit)
{
	if(Broodwar->isReplay()) return;
}

void SIBot::onUnitMorph(BWAPI::Unit* unit)
{
	if(Broodwar->isReplay()) return;
}

void SIBot::onUnitRenegade(BWAPI::Unit* unit)
{
	if(Broodwar->isReplay()) return;
}

void SIBot::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
}