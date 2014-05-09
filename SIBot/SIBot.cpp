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
	frameWaits.push_back(0);
	frameWaits.push_back(0);
	move = false;
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
		m_qTable->initialize(m_stateMaster->getStateSize(), 3);
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
	for(std::vector<int>::size_type j = 0; j != frameWaits.size(); j++)
	{
		if(frameWaits.at(j)>0)
		{
			frameWaits.at(j)--;
		}
	}
	if (Broodwar->isReplay()) return;
	if(m_enemies.empty()) return;
	int i = 0;
	for(std::set<Unit*>::iterator it = m_heroes.begin(); it!=m_heroes.end(); ++it)
	{

		if(readyForAction(*it, i))
		{
			int previousEnemyHP = m_stateMaster->getEnemyHP();
			int previousAlliedHP = m_stateMaster->getAlliedHP();
			int currentState = m_stateMaster->observe((*it), m_enemies);

			if(m_stateMaster->getPrevious()!= -1)
			{
				updateAlliedHealth();
				rewardAgentByQ(previousEnemyHP, previousAlliedHP, currentState, (*it)->isStimmed());
			}
			else
			{
				updateAlliedHealth();
			}
			int action = m_policy.chooseGreedyAction(currentState, m_qTable, (*it));
			if(action == 0)
			{
				Broodwar->sendText("FIGHT");
				Action::fight((*it), m_enemies);
				if((*it)->isStimmed()) frameWaits.at(i) = 5;
				frameWaits.at(i) = 8;
				move = false;
			}
			else if(action == 1) 
			{
				Broodwar->sendText("RUN");
				Action::run((*it), m_enemies);
				if((*it)->isStimmed()) frameWaits.at(i) = 6;
				frameWaits.at(i) = 12;
				move = true;
			}
			else 
			{
				Broodwar->sendText("STIM!");
				Action::useAbility((*it));
				move = false;
			}
			m_stateMaster->setPrevious(currentState);
			m_stateMaster->setPreviousA(action);
		}
		i++;
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

void SIBot::rewardAgentByQ(int previousEnemyHP, int previousAlliedHP, int currentState, bool cd)
{
	int i = m_stateMaster->getPrevious();
	int j = m_stateMaster->getPreviousA();
	int r = (previousEnemyHP - m_stateMaster->getEnemyHP()) - (previousAlliedHP - m_stateMaster->getAlliedHP());
	double oldValue = m_qTable->getValue(i, j);
	double updateValue = oldValue + 0.9 * (r + 0.8*m_qTable->maxStateValue(currentState, cd) - oldValue);
	m_qTable->updateTable(i, j, updateValue);
}

void SIBot::rewardAgentBySarsa(int previousEnemyHP, int previousAlliedHP, int currentState, BWAPI::Unit* hero)
{
	int i = m_stateMaster->getPrevious();
	int j = m_stateMaster->getPreviousA();
	int r = (previousEnemyHP - m_stateMaster->getEnemyHP()) - (previousAlliedHP - m_stateMaster->getAlliedHP());
	double oldValue = m_qTable->getValue(i, j);
	int action = m_policy.chooseGreedyAction(currentState, m_qTable, hero);
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

bool SIBot::readyForAction(BWAPI::Unit* unit, int i)
{
	if(move && frameWaits.at(i) == 0)
	{
		return true;
	}
	if(!unit->isAttackFrame() && !unit->isMoving() && frameWaits.at(i) == 0)
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