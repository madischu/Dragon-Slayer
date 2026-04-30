#ifndef DRAGONS_LAIR_H
#define DRAGONS_LAIR_H

#include "Location.h"
#include "Player.h"

class DragonsLair : public Location
{
public:
    DragonsLair();
    bool enter(Player& player);
};

#endif
