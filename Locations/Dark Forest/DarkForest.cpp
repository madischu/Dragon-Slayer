#include "DarkForest.h"
#include "../../Main Game/ConsoleColor.h"
#include "../../Main Game/ConsoleInput.h"
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

std::string DarkForest::getQuestMenuText() const
{
    return
        "\n1: Accept this Quest\n"
        "2: Reject this Quest\n"
        "3: Present Items";
}

std::string DarkForest::acceptQuest(ActionStack& actionLog)
{
    actionLog.push("Accepted the wizard quest prompt");
    return "\nQuests coming soon! There are no quests available at this time. Try again later.";
}

std::string DarkForest::rejectQuest(ActionStack& actionLog)
{
    actionLog.push("Rejected the wizard quest prompt");
    return "\nThe wizard nods, bids you farewell, and retreats back into the shadows.";
}

std::string DarkForest::presentItems(ActionStack& actionLog)
{
    actionLog.push("Presented items to the wizard");
    return "\nFeature Coming soon!";
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

        while (true)
        {
            ConsoleColor::printLine(getQuestMenuText(), ConsoleColor::Color::White);

            int choice;
            if (!ConsoleInput::readInt(choice))
            {
                return;
            }

            if (choice == 1)
            {
                ConsoleColor::printLine(acceptQuest(engine.getActionLog()), ConsoleColor::Color::DarkGreen);
            }
            else if (choice == 2)
            {
                ConsoleColor::printLine(rejectQuest(engine.getActionLog()), ConsoleColor::Color::DarkGreen);
                return;
            }
            else if (choice == 3)
            {
                ConsoleColor::printLine(presentItems(engine.getActionLog()), ConsoleColor::Color::DarkGreen);
            }
            else
            {
                ConsoleInput::printInvalidInput();
                engine.addAction("Entered invalid wizard quest choice");
            }
        }
    }

    ConsoleColor::printLine("\nYou are in the Dark Forest. It is quiet here.", ConsoleColor::Color::DarkGreen);
    engine.addAction("Explored the Dark Forest");
}
