#pragma once
#include <BWAPI.h>
#include <limits>

class Action
{
public:
	static void fight(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies);
	static void run(BWAPI::Unit* hero, std::set<BWAPI::Unit*> enemies);
	static void useAbility(BWAPI::Unit* hero);
    static float distance(BWAPI::Position a, BWAPI::Position b);
	static float distance(int x, int y, BWAPI::Position b);
};