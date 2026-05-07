#include "DragonsLair.h"
#include "../../Main Game/ConsoleColor.h"
#include "CombatSystem.h"
#include <iostream>

DragonsLair::DragonsLair()
{
}

bool DragonsLair::enter(Player& player)
{
    ConsoleColor::printLine("You entered the Dragon's Lair. Prepare for battle!", ConsoleColor::Color::LightPurple);

    Monster dragon("Dragon", 20, 300, "sharp");
    return CombatSystem::fight(player, dragon, false);
}
