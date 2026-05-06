#include "AncientRuins.h"
#include "../../Main Game/ConsoleColor.h"

#include <iostream>

AncientRuins::AncientRuins()
{
}

void AncientRuins::enter(GameEngine& engine)
{
    ConsoleColor::printLine("\nYou explore the Ancient Ruins. You find nothing here.", ConsoleColor::Color::Brown);
    engine.addAction("Explored the Ancient Ruins");
}
