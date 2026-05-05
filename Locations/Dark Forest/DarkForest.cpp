#include "DarkForest.h"
#include "../../Main Game/ConsoleColor.h"
#include "../../Main Game/GameEngine.h"

#include <cstdlib>
#include <iostream>

DarkForest::DarkForest()
{
}

void DarkForest::rollForWizardEncounter(ActionStack& actionLog)
{
    pendingWizardEncounter = (rand() % 100) < 30;

    if (pendingWizardEncounter)
    {
        actionLog.push("Ran into the forest wizard");
    }
}

void DarkForest::handleTravelArrival(ActionStack& actionLog, std::string& message)
{
    rollForWizardEncounter(actionLog);

    if (pendingWizardEncounter)
    {
        message += "\nYou notice a cloaked figure waiting among the trees.";
    }
}

bool DarkForest::hasPendingWizardEncounter(const std::string& currentLocation) const
{
    return pendingWizardEncounter && currentLocation == "Dark Forest";
}

std::string DarkForest::interactWithWizard(Player& player, ActionStack& actionLog, const std::string& currentLocation)
{
    if (!hasPendingWizardEncounter(currentLocation))
    {
        return "You are in the Dark Forest. It is quiet here.";
    }

    pendingWizardEncounter = false;
    player.addHealth(20);
    actionLog.push("Interacted with the forest wizard");

    return "A robed wizard steps out from shadows. He is no enemy, he would like to send you on a quest.";
}

void DarkForest::clearEncounter()
{
    pendingWizardEncounter = false;
}

void DarkForest::enter(GameEngine& engine)
{
    if (hasPendingWizardEncounter(engine.getCurrentLocation()))
    {
        ConsoleColor::printLine("\n" + interactWithWizard(engine.getPlayer(), engine.getActionLog(), engine.getCurrentLocation()), ConsoleColor::Color::DarkGreen);
        return;
    }

    ConsoleColor::printLine("\nYou are in the Dark Forest. It is quiet here.", ConsoleColor::Color::DarkGreen);
    engine.addAction("Explored the Dark Forest");
}
