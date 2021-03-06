#pragma once
#include "StateMastermind.h"
#include "QTable.h"
#include "Policy.h"
#include <BWAPI.h>

class SIBot : public BWAPI::AIModule
{
public:
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onUnitDiscover(BWAPI::Unit* unit);
	virtual void onUnitEvade(BWAPI::Unit* unit);
	virtual void onUnitMorph(BWAPI::Unit* unit);
	virtual void onUnitRenegade(BWAPI::Unit* unit);
	virtual void onUnitDestroy(BWAPI::Unit* unit);
	virtual void onSendText(std::string text);
	~SIBot();
	void showPlayers();
	void showForces();
	bool analyzed;
	void updateAlliedHealth();

private:
	void appendRecords(int winner);
	void rewardAgentByQ(int preEnemyHP, int preAlliedHP, int currentState, bool cd);
	void rewardAgentBySarsa(int preEnemyHP, int preAlliedHP, int currentState, BWAPI::Unit* hero);
	bool readyForAction(BWAPI::Unit* unit, int i);

	std::set<BWAPI::Unit*> m_heroes;
	std::set<BWAPI::Unit*> m_enemies;
	StateMastermind* m_stateMaster;
	QTable* m_qTable;
	Policy m_policy;
	int frameWait;
	bool move;
	std::vector<int> frameWaits;
};