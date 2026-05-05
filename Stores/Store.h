#ifndef STORE_H
#define STORE_H

#include "ActionStack.h"
#include "Player.h"

class Store
{
public:
    Store();

    void enter(Player& player, ActionStack& actionLog);
    void buyHealth(Player& player, ActionStack& actionLog);
    void buyWeapon(Player& player, ActionStack& actionLog);
    void sellWeapon(Player& player, ActionStack& actionLog);
    void buyPotion(Player& player, ActionStack& actionLog);
    void buyArmor(Player& player, ActionStack& actionLog);
};

#endif
