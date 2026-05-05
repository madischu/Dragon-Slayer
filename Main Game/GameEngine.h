#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "ActionStack.h"
#include "../Locations/Dark Forest/DarkForest.h"
#include "Player.h"
#include "WorldMap.h"

#include <string>
#include <utility>
#include <vector>

class GameEngine
{
private:
    Player player;
    WorldMap worldMap;
    ActionStack actionLog;
    DarkForest darkForest;

public:
    GameEngine();

    Player& getPlayer();
    const Player& getPlayer() const;

    WorldMap& getWorldMap();
    const WorldMap& getWorldMap() const;

    ActionStack& getActionLog();
    const ActionStack& getActionLog() const;

    std::string getStatusText() const;
    std::string getStatusLine() const;
    std::string getInventoryText() const;
    std::string getLogText() const;
    std::string getCurrentLocation() const;
    std::vector<std::string> getAvailablePaths() const;
    std::vector<std::pair<std::string, std::string>> getMapPaths() const;
    bool hasPendingWizardEncounter() const;
    std::string interactWithWizard();
    void enterDarkForest();

    void addAction(const std::string& action);
    bool travelToPathIndex(int pathChoice, std::string& message);
    bool travelToLocation(const std::string& locationName, std::string& message);

private:
    void handleCompletedTravel(const std::string& previousLocation, std::string& message);
};

#endif
