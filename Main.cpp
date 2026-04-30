#include "Game.h"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));

    Game game;
    game.start();

    return 0;
}
