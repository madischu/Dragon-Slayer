#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Store.h"
#include "Caves.h"
#include "DragonsLair.h"

class Game
{
private:
    Player player;
    Store store;
    Caves caves;
    DragonsLair dragonsLair;
    bool running;

public:
    Game();

    void start();
    void showMainMenu();
    void restart();
    void winGame();
    void loseGame();
};

#endif
