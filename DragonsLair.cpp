#include "DragonsLair.h"
#include "CombatSystem.h"
#include <iostream>

DragonsLair::DragonsLair()
    : Location("Dragon's Lair", "You enter the Dragon's Lair.")
{
}

bool DragonsLair::enter(Player& player)
{
    Location::enter();
    std::cout << "You entered the Dragon's Lair. Prepare for battle!" << std::endl;

    Monster dragon("Dragon", 20, 300, "sharp");
    return CombatSystem::fight(player, dragon, false);
}
