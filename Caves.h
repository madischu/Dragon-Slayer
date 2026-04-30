#ifndef CAVES_H
#define CAVES_H

#include "Location.h"
#include "Player.h"

class Caves : public Location
{
public:
    Caves();
    int enter(Player& player);
};

#endif
