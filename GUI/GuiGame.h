#ifndef GUIGAME_H
#define GUIGAME_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class GuiMode
{
    Main,
    Store,
    Combat,
    CaveChoice,
    WizardQuest
};

class GuiGameImpl;

class GuiGame
{
public:
    GuiGame();
    ~GuiGame();

    std::string getStatusText() const;
    GuiMode getMode() const;
    std::string getCurrentLocation() const;
    std::vector<std::string> getAvailablePaths() const;
    std::vector<std::pair<std::string, std::string>> getMapPaths() const;
    bool travelToLocation(const std::string& locationName, std::string& message);

    std::string interact();
    std::string getQuestsText();
    std::string getInventoryText();
    std::string getLogText() const;

    std::string buyHealth();
    std::string buyWeapon();
    std::string sellWeapon();
    std::string buyPotion();
    std::string buyArmor();
    std::string leaveStore();

    std::string attackMonster();
    std::string dodgeMonster();
    std::string useHealthPotion();
    std::string runFromCombat();
    std::string fightSlime();
    std::string fightFangedBeast();
    std::string leaveCaves();
    std::string acceptWizardQuest();
    std::string rejectWizardQuest();
    std::string presentWizardItems();

    std::string getStoreText() const;
    std::string getCombatText() const;

private:
    std::unique_ptr<GuiGameImpl> impl;
};

#endif
