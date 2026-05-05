#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Player.h"
#include "Monster.h"

class CombatSystem
{
public:
    static bool fight(Player& player, Monster& monster, bool canRunAway, bool canWeaponBreak = true);
    static void playerAttack(Player& player, Monster& monster);
};

#endif
