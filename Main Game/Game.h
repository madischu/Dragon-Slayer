#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Store.h"
#include "Caves.h"
#include "DragonsLair.h"
#include "WorldMap.h"
#include "EnemyQueue.h"
#include "ActionStack.h"
#include <string>

class Game
{
private:
    Player player;
    Store store;
    Caves caves;
    DragonsLair dragonsLair;
    
    WorldMap worldMap;
    ActionStack actionLog;
    
    bool running;

public:
    Game();

    void start();
    void processLocation();
    void travel();
    void fightEnemyWave(EnemyQueue& enemies);
    void fightSingleMonster(Monster& monster);
    void displayMainMenu();
    void restart();
    void winGame();
    void loseGame();
};

#endif