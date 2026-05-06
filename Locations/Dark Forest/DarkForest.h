#ifndef DARKFOREST_H
#define DARKFOREST_H

#include <string>

class ActionStack;
class GameEngine;
class Player;

class DarkForest
{
private:
    bool pendingWizardEncounter = false;

public:
    DarkForest();

    void rollForWizardEncounter(ActionStack& actionLog);
    void handleTravelArrival(ActionStack& actionLog, std::string& message);
    bool hasPendingWizardEncounter(const std::string& currentLocation) const;
    std::string interactWithWizard(Player& player, ActionStack& actionLog, const std::string& currentLocation);
    std::string getQuestMenuText() const;
    std::string acceptQuest(ActionStack& actionLog);
    std::string rejectQuest(ActionStack& actionLog);
    std::string presentItems(ActionStack& actionLog);
    void clearEncounter();
    void enter(GameEngine& engine);
};

#endif
