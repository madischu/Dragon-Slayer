#include "AncientRuins.h"

#include <iostream>

AncientRuins::AncientRuins()
{
}

void AncientRuins::enter(GameEngine& engine)
{
    std::cout << "\nYou explore the Ancient Ruins. There are no enemies here." << std::endl;
    engine.addAction("Explored the Ancient Ruins");
}
