#ifndef STORE_H
#define STORE_H

#include "Location.h"
#include "Player.h"

class Store : public Location
{
public:
    Store();

    void enter(Player& player);
    void buyHealth(Player& player);
    void buyWeapon(Player& player);
    void sellWeapon(Player& player);
};

#endif
