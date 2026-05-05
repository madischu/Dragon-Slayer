#include "DragonsLair.h"
#include "CombatSystem.h"
#include <iostream>

DragonsLair::DragonsLair()
{
}

bool DragonsLair::enter(Player& player)
{
    std::cout << "You entered the Dragon's Lair. Prepare for battle!" << std::endl;

    Monster dragon("Dragon", 20, 300, "sharp");
    return CombatSystem::fight(player, dragon, false);
}
