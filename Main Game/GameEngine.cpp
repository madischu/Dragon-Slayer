#include "GameEngine.h"

#include <sstream>

GameEngine::GameEngine()
{
    worldMap.buildDefaultMap();
}

Player& GameEngine::getPlayer()
{
    return player;
}

const Player& GameEngine::getPlayer() const
{
    return player;
}

WorldMap& GameEngine::getWorldMap()
{
    return worldMap;
}

const WorldMap& GameEngine::getWorldMap() const
{
    return worldMap;
}

ActionStack& GameEngine::getActionLog()
{
    return actionLog;
}

const ActionStack& GameEngine::getActionLog() const
{
    return actionLog;
}

// CLI
std::string GameEngine::getStatusText() const
{
    std::ostringstream output;
    output << "\n" << player.getStatsText();
    return output.str();
}

// GUI
std::string GameEngine::getStatusLine() const
{
    std::ostringstream output;
    output << "    Location: " << worldMap.getCurrentLocation()
           << "    Health: " << player.getHealth()
           << "    Gold: " << player.getGold()
           << "    XP: " << player.getXP()
           << "    Weapon: " << player.getCurrentWeapon().getName()
           << "    Items: " << player.getInventory().size();
    return output.str();
}

std::string GameEngine::getInventoryText() const
{
    return player.getInventory().getInventoryText();
}

std::string GameEngine::getLogText() const
{
    return actionLog.getLogText();
}

std::string GameEngine::getCurrentLocation() const
{
    return worldMap.getCurrentLocation();
}

std::vector<std::string> GameEngine::getAvailablePaths() const
{
    return worldMap.getAvailablePaths();
}

std::vector<std::pair<std::string, std::string>> GameEngine::getMapPaths() const
{
    return worldMap.getAllPaths();
}

bool GameEngine::hasPendingWizardEncounter() const
{
    return darkForest.hasPendingWizardEncounter(worldMap.getCurrentLocation());
}

std::string GameEngine::interactWithWizard()
{
    return darkForest.interactWithWizard(player, actionLog, worldMap.getCurrentLocation());
}

void GameEngine::enterDarkForest()
{
    darkForest.enter(*this);
}

std::string GameEngine::getWizardQuestMenuText() const
{
    return darkForest.getQuestMenuText();
}

std::string GameEngine::acceptWizardQuest()
{
    return darkForest.acceptQuest(actionLog);
}

std::string GameEngine::rejectWizardQuest()
{
    return darkForest.rejectQuest(actionLog);
}

std::string GameEngine::presentWizardItems()
{
    return darkForest.presentItems(actionLog);
}

void GameEngine::addAction(const std::string& action)
{
    actionLog.push(action);
}

bool GameEngine::travelToPathIndex(int pathChoice, std::string& message)
{
    std::string previousLocation = worldMap.getCurrentLocation();

    if (!worldMap.moveToLocation(pathChoice))
    {
        message = "Invalid path choice.";
        return false;
    }

    handleCompletedTravel(previousLocation, message);

    return true;
}

bool GameEngine::travelToLocation(const std::string& locationName, std::string& message)
{
    std::string previousLocation = worldMap.getCurrentLocation();

    if (!worldMap.moveToLocationByName(locationName, message))
    {
        return false;
    }

    handleCompletedTravel(previousLocation, message);

    return true;
}

void GameEngine::handleCompletedTravel(const std::string& previousLocation, std::string& message)
{
    std::string newLocation = worldMap.getCurrentLocation();
    actionLog.push("Moved from " + previousLocation + " to " + newLocation);

    if (newLocation == "Dark Forest")
    {
        darkForest.handleTravelArrival(actionLog, message);
    }
    else
    {
        darkForest.clearEncounter();
    }
}
