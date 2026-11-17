#ifndef COMBATACTION_HPP
#define COMBATACTION_HPP
#include <iostream>
using namespace std;

// fight actions used by the enemy AI 
enum CombatAction{
    ACTION_ADVANCE,
    ACTION_RETREAT,
    ACTION_QUICK_ATTACK,
    ACTION_POWER_ATTACK,
    ACTION_SPECIAL,
    ACTION_REST
};
#endif
