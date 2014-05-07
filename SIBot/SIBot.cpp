#include "SIBot.h"
#include "Action.h"
#include "../../Addons/Util.h"
#include "../../include/BWTA.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace BWAPI;

void SIBot::onStart()
{
	if(Broodwar->isReplay()) return;
	m_stateMaster = 0;
	m_qTable = 0;
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
	m_qTable = new QTable();
	if(m_qTable!=0)
	{
		Broodwar->sendText("Spanish inquisition online for realzorz");
		m_qTable->initialize(m_stateMaster->getStateSize(), 2);
	}
}

SIBot::~SIBot()
{
}

void SIBot::onEnd(bool isWinner)
{
	if(isWinner)
	{
		appendRecords(1);
	}
	else
	{
		appendRecords(0);
	}
	Broodwar->sendText("Spanish inquisition offline for realzorz");
	m_qTable->shutdown();
	m_qTable = 0;
	m_policy.saveConstants();
	Broodwar->sendText("Qtable saved to qtable.txt...");
	m_stateMaster->destroy();
	m_stateMaster = 0;
}

void SIBot::onFrame()
{
	if (Broodwar->isReplay()) return;
	if(m_enemies.empty()) return;
	for(std::set<Unit*>::iterator it = m_heroes.begin(); it!=m_heroes.end(); ++it)
	{
		if(readyForAction(*it))
		{
			int previousEnemyHP = m_stateMaster->getEnemyHP();
			int previousAlliedHP = m_stateMaster->getAlliedHP();
			int currentState = m_stateMaster->observe((*it), m_enemies);

			if(m_stateMaster->getPrevious()!= -1)
			{
				updateAlliedHealth();
				rewardAgentBySarsa(previousEnemyHP, previousAlliedHP, currentState);
			}
			else
			{
				updateAlliedHealth();
			}
			//std::stringstream sstr;
			//sstr << currentState;
			//std::string str1 = sstr.str();
			//Broodwar->sendText(str1.c_str());
			int action = m_policy.chooseGreedyAction(currentState, m_qTable);
			if(action == 0)
			{
				Broodwar->sendText("FIGHT");
				Action::fight((*it), m_enemies);
			}
			//}
			else 
			{
				Broodwar->sendText("RUN");
				Action::run((*it), m_enemies);
			}
			m_stateMaster->setPrevious(currentState);
			m_stateMaster->setPreviousA(action);
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

void SIBot::rewardAgentByQ(int previousEnemyHP, int previousAlliedHP, int currentState)
{
	int i = m_stateMaster->getPrevious();
	int j = m_stateMaster->getPreviousA();
	int r = (previousEnemyHP - m_stateMaster->getEnemyHP()) - (previousAlliedHP - m_stateMaster->getAlliedHP());
	double oldValue = m_qTable->getValue(i, j);
	double updateValue = oldValue + 0.9 * (r + 0.8*m_qTable->maxStateValue(currentState) - oldValue);
	m_qTable->updateTable(i, j, updateValue);
}

void SIBot::rewardAgentBySarsa(int previousEnemyHP, int previousAlliedHP, int currentState)
{
	int i = m_stateMaster->getPrevious();
	int j = m_stateMaster->getPreviousA();
	int r = (previousEnemyHP - m_stateMaster->getEnemyHP()) - (previousAlliedHP - m_stateMaster->getAlliedHP());
	double oldValue = m_qTable->getValue(i, j);
	int action = m_policy.chooseGreedyAction(currentState, m_qTable);
	double updateValue = oldValue + 0.9 * (r + 0.8*m_qTable->getValue(currentState, action) - oldValue);
	m_qTable->updateTable(i, j, updateValue);
}
void SIBot::updateAlliedHealth()
{
	int hp = 0;
	for(std::set<Unit*>::iterator it = m_heroes.begin(); it!=m_heroes.end(); ++it)
	{
		hp += (*it)->getHitPoints();
	}
	m_stateMaster->setAlliedHP(hp);
}

bool SIBot::readyForAction(BWAPI::Unit* unit)
{
	if(!unit->isAttackFrame() && !unit->isMoving() && !unit->isStartingAttack() && !unit->isAttacking())
	{
		return true;
	}
	return false;
}

void SIBot::appendRecords(int winner)
{
	std::ofstream log("results.txt", std::ios_base::app | std::ios_base::out);
	log << m_policy.getEpisode() << " " << winner << "\n";
	log.close();
}