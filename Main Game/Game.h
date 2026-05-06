#ifndef GAME_H
#define GAME_H

#include "../Locations/Ancient Ruins/AncientRuins.h"
#include "Caves.h"
#include "DragonsLair.h"
#include "GameEngine.h"
#include "Store.h"

class Game
{
private:
    AncientRuins ancientRuins;
    Caves caves;
    DragonsLair dragonsLair;
    GameEngine engine;
    Store store;
    bool running;

public:
    Game();

    void start();
    void processLocation();
    void travel();
    void displayMainMenu();
};

#endif
