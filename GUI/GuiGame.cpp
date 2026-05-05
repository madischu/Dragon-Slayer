#include "GuiGame.h"

#include "../Main Game/GameEngine.h"
#include "Monster.h"

#include <cstdlib>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class GuiGameImpl
{
public:
    GuiGameImpl()
    {
        engine.addAction("Started a new game");
    }

    std::string getStatusText() const
    {
        return engine.getStatusLine();
    }

    GuiMode getMode() const
    {
        return mode;
    }

    std::string getCurrentLocation() const
    {
        return engine.getCurrentLocation();
    }

    std::vector<std::string> getAvailablePaths() const
    {
        return engine.getAvailablePaths();
    }

    std::vector<std::pair<std::string, std::string>> getMapPaths() const
    {
        return engine.getMapPaths();
    }

    bool travelToLocation(const std::string& locationName, std::string& message)
    {
        return engine.travelToLocation(locationName, message);
    }

    std::string interact()
    {
        std::string location = engine.getCurrentLocation();

        if (location == "Town Square")
        {
            engine.addAction("Visited Town Square");
            return "You are in the Town Square. You see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'.";
        }

        if (location == "Store")
        {
            mode = GuiMode::Store;
            engine.addAction("Visited Store");
            return getStoreText();
        }

        if (location == "Dragon's Lair")
        {
            std::vector<Monster> monsters =
            {
                Monster("Dragon", 35, 300, "sharp")
            };

            return startCombat(monsters, "Entered the Dragon's Lair");
        }

        if (location == "Caves")
        {
            if ((rand() % 100) >= 25)
            {
                mode = GuiMode::CaveChoice;
                engine.addAction("Entered the caves");
                return getCaveChoiceText();
            }

            std::vector<Monster> monsters =
                buildCaveWaveForCurrentWeapon();

            return startCombat(monsters, "Started a cave enemy wave");
        }

        if (location == "Dark Forest")
        {
            if (engine.hasPendingWizardEncounter())
            {
                return engine.interactWithWizard();
            }

            engine.addAction("Explored the Dark Forest");
            return "You are in the Dark Forest. It is quiet here.";
        }

        if (location == "Ancient Ruins")
        {
            engine.addAction("Explored the Ancient Ruins");
            return "You explore the Ancient Ruins. There are no enemies here.";
        }

        engine.addAction("Explored " + location + " from GUI");
        return "There is nothing else to do here right now.";
    }

    std::string getQuestsText()
    {
        return "View Quests Here";
    }

    std::string getInventoryText()
    {
        return engine.getInventoryText();
    }

    std::string buyHealth()
    {
        if (engine.getPlayer().getHealth() == 300)
        {
            return getStoreText("Your health is already at the maximum.");
        }

        if (engine.getPlayer().getGold() < 10)
        {
            return getStoreText("Not enough gold.");
        }

        engine.getPlayer().subtractGold(10);
        engine.getPlayer().addHealth(10);
        engine.addAction("Bought 10 health");
        return getStoreText("You bought 10 health.");
    }

    std::string buyWeapon()
    {
        Inventory& inventory = engine.getPlayer().getInventory();

        if (engine.getPlayer().getGold() < 30)
        {
            return getStoreText("Not enough gold.");
        }

        if (!inventory.containsItem("Dagger"))
        {
            engine.getPlayer().subtractGold(30);
            inventory.addItem(Item("Dagger", ItemType::Weapon, 30, "Sharp starter weapon"));
            engine.getPlayer().equipWeapon("Dagger");
            engine.addAction("Bought Dagger");
            return getStoreText("You bought and equipped the Dagger.");
        }

        if (!inventory.containsItem("Claw Hammer"))
        {
                if (engine.getPlayer().getXP() < 50)
                {
                    return getStoreText("You need at least 50 XP to purchase the Claw Hammer.");
                }

            engine.getPlayer().subtractGold(30);
            inventory.addItem(Item("Claw Hammer", ItemType::Weapon, 50, "Strong blunt weapon"));
                engine.getPlayer().equipWeapon("Claw Hammer");
                engine.addAction("Bought Claw Hammer");
                return getStoreText("You bought and equipped the Claw Hammer.");
            }

        if (!inventory.containsItem("Sword"))
        {
                if (engine.getPlayer().getXP() < 150)
                {
                    return getStoreText("You need at least 150 XP to purchase the Sword.");
                }

            engine.getPlayer().subtractGold(30);
            inventory.addItem(Item("Sword", ItemType::Weapon, 100, "Required to defeat the Dragon"));
                engine.getPlayer().equipWeapon("Sword");
                engine.addAction("Bought Sword");
                return getStoreText("You bought and equipped the Sword.");
            }

        return getStoreText("You already have the strongest weapon.");
    }

    std::string sellWeapon()
    {
        Inventory& inventory = engine.getPlayer().getInventory();
        Weapon currentWeapon = engine.getPlayer().getCurrentWeapon();

        if (currentWeapon.getName() == "Stick")
        {
            return getStoreText("You can't sell your basic weapon.");
        }

        if (!inventory.removeItem(currentWeapon.getName()))
        {
            return getStoreText("Could not sell weapon.");
        }

        engine.getPlayer().addGold(15);
        engine.getPlayer().equipWeapon("Stick");
        engine.addAction("Sold " + currentWeapon.getName());
        return getStoreText("You sold your " + currentWeapon.getName() + ".");
    }

    std::string buyPotion()
    {
        if (engine.getPlayer().getGold() < 15)
        {
            return getStoreText("Not enough gold.");
        }

        engine.getPlayer().subtractGold(15);
        engine.getPlayer().addItemToInventory(Item("Health Potion", ItemType::Potion, 50, "Restores 50 health"));
        engine.addAction("Bought Health Potion");
        return getStoreText("You bought a Health Potion.");
    }

    std::string buyArmor()
    {
        Inventory& inventory = engine.getPlayer().getInventory();

        if (inventory.containsItem("Leather Armor"))
        {
            return getStoreText("You already own Leather Armor.");
        }

        if (engine.getPlayer().getGold() < 40)
        {
            return getStoreText("Not enough gold.");
        }

        engine.getPlayer().subtractGold(40);
        engine.getPlayer().addItemToInventory(Item("Leather Armor", ItemType::Armor, 10, "Basic armor"));
        engine.addAction("Bought Leather Armor");
        return getStoreText("You bought Leather Armor.");
    }

    std::string leaveStore()
    {
        mode = GuiMode::Main;
        engine.addAction("Left Store");
        return "Leaving the store.";
    }

    std::string attackMonster()
    {
        if (mode != GuiMode::Combat || combatIndex >= combatMonsters.size())
        {
            mode = GuiMode::Main;
            return "There is no enemy to attack.";
        }

        Monster& monster = combatMonsters[combatIndex];

        if (!engine.getPlayer().canFightMonster(monster))
        {
            engine.addAction("Could not fight " + monster.getName() + " because XP was too low");
            mode = GuiMode::Main;
            return "You are not strong enough to fight the " + monster.getName() + ".";
        }

        Weapon currentWeapon = engine.getPlayer().getCurrentWeapon();
        int bonus = engine.getPlayer().getXP() / 10;
        int baseDamage = currentWeapon.getPower();
        int totalDamage = baseDamage + bonus;
        int effectiveness = engine.getPlayer().getWeaponEffectiveness(currentWeapon, monster);
        int finalDamage = 0;

        if (baseDamage > 0)
        {
            finalDamage = (totalDamage * effectiveness) / baseDamage;
        }

        std::ostringstream output;
        output << "You are fighting a " << monster.getName() << ".\r\n";

        if (engine.getPlayer().isMonsterHit())
        {
            monster.takeDamage(finalDamage, currentWeapon.getType());
            output << "You attacked with " << currentWeapon.getName()
                   << " and dealt " << finalDamage << " damage.\r\n";
            engine.addAction("Attacked " + monster.getName() + " with " + currentWeapon.getName());
        }
        else
        {
            output << "You missed.\r\n";
            engine.addAction("Missed an attack against " + monster.getName());
        }

        if (monster.getHealth() <= 0)
        {
            engine.getPlayer().defeatMonster(monster);
            engine.addAction("Defeated " + monster.getName());
            output << "You defeated the " << monster.getName() << ".\r\n";
            return advanceCombat(output.str());
        }

        output << monsterAttack(monster);
        output << "\r\n" << getCombatText();
        return output.str();
    }

    std::string dodgeMonster()
    {
        if (mode != GuiMode::Combat || combatIndex >= combatMonsters.size())
        {
            mode = GuiMode::Main;
            return "There is no enemy to dodge.";
        }

        Monster& monster = combatMonsters[combatIndex];
        std::ostringstream output;

        if (engine.getPlayer().dodge(monster))
        {
            engine.addAction("Dodged " + monster.getName());
            output << "You dodged the " << monster.getName() << ".";
        }
        else
        {
            output << "You failed to dodge.\r\n";
            output << monsterAttack(monster);
        }

        output << "\r\n" << getCombatText();
        return output.str();
    }

    std::string useHealthPotion()
    {
        if (!engine.getPlayer().getInventory().containsItem("Health Potion"))
        {
            return "You do not have that potion.";
        }

        engine.getPlayer().usePotion("Health Potion");
        engine.addAction("Used Health Potion");

        if (mode == GuiMode::Combat)
        {
            return "You used a Health Potion.\r\n" + getCombatText();
        }

        return "You used a Health Potion.";
    }

    std::string runFromCombat()
    {
        if (mode != GuiMode::Combat || combatIndex >= combatMonsters.size())
        {
            mode = GuiMode::Main;
            return "There is no battle to run from.";
        }

        std::string monsterName = combatMonsters[combatIndex].getName();
        mode = GuiMode::Main;
        combatMonsters.clear();
        combatIndex = 0;
        engine.addAction("Ran away from " + monsterName);
        return "You ran away from the battle.";
    }

    std::string fightSlime()
    {
        if (mode != GuiMode::CaveChoice)
        {
            return "There is no cave enemy to choose right now.";
        }

        std::vector<Monster> monsters =
        {
            Monster("Slime", 2, 15, "blunt")
        };

        return startCombat(monsters, "Chose to fight Slime in the caves");
    }

    std::string fightFangedBeast()
    {
        if (mode != GuiMode::CaveChoice)
        {
            return "There is no cave enemy to choose right now.";
        }

        std::vector<Monster> monsters =
        {
            Monster("Fanged Beast", 8, 60, "sharp")
        };

        return startCombat(monsters, "Chose to fight Fanged Beast in the caves");
    }

    std::string leaveCaves()
    {
        if (mode == GuiMode::CaveChoice)
        {
            mode = GuiMode::Main;
            engine.addAction("Left the caves without fighting");
        }

        return "Leaving the caves.";
    }

    std::string getStoreText() const
    {
        return "Would you like to make a purchase?\r\n"
               "Health: 10 gold\r\n"
               "Weapon: 30 gold\r\n"
               "Sell Weapon: +15 gold\r\n"
               "Potion: 15 gold\r\n"
               "Armor: 40 gold\r\n";
    }

    std::string getCombatText() const
    {
        if (mode != GuiMode::Combat || combatIndex >= combatMonsters.size())
        {
            return "No active battle.";
        }

        const Monster& monster = combatMonsters[combatIndex];
        std::ostringstream output;
        output << "Enemy: " << monster.getName() << "\r\n"
               << "Monster Health: " << monster.getHealth() << "\r\n"
               << "Choose Attack, Dodge, Potion, or Run Away.";
        return output.str();
    }

    std::string getCaveChoiceText() const
    {
        return "Choose a cave enemy to fight.";
    }

    std::string getLogText() const
    {
        return engine.getLogText();
    }

private:
    GameEngine engine;
    GuiMode mode = GuiMode::Main;
    std::vector<Monster> combatMonsters;
    int combatIndex = 0;

    std::string getStoreText(const std::string& message) const
    {
        return message + "\r\n\r\n" + getStoreText();
    }

    std::string startCombat(const std::vector<Monster>& monsters, const std::string& action)
    {
        combatMonsters = monsters;
        combatIndex = 0;
        mode = GuiMode::Combat;
        engine.addAction(action);

        if (combatMonsters.empty())
        {
            mode = GuiMode::Main;
            return "There are no enemies here.";
        }

        engine.addAction("Encountered " + combatMonsters[combatIndex].getName());
        return "Enemy wave started.\r\n" + getCombatText();
    }

    std::vector<Monster> buildCaveWaveForCurrentWeapon() const
    {
        std::vector<Monster> monsters;

        if (engine.getPlayer().getCurrentWeapon().getType() == "sharp")
        {
            monsters.push_back(Monster("Fanged Beast", 8, 60, "sharp"));
            monsters.push_back(Monster("Fanged Beast", 8, 60, "sharp"));
        }
        else
        {
            monsters.push_back(Monster("Slime", 2, 15, "blunt"));
            monsters.push_back(Monster("Slime", 2, 15, "blunt"));
        }

        return monsters;
    }

    std::string monsterAttack(Monster& monster)
    {
        int damage = monster.attack();
        engine.getPlayer().takeDamage(damage);
        engine.addAction(monster.getName() + " attacked the player");

        std::ostringstream output;
        output << "The " << monster.getName() << " attacked for " << damage
               << " damage. Current health: " << engine.getPlayer().getHealth() << ".";

        if (engine.getPlayer().getHealth() <= 0)
        {
            mode = GuiMode::Main;
            combatMonsters.clear();
            combatIndex = 0;
            engine.addAction("Player lost the game");
            output << "\r\nYou have been defeated.";
        }

        return output.str();
    }

    std::string advanceCombat(const std::string& previousMessage)
    {
        ++combatIndex;

        if (combatIndex >= static_cast<int>(combatMonsters.size()))
        {
            mode = GuiMode::Main;
            combatMonsters.clear();
            combatIndex = 0;
            engine.addAction("Completed an enemy wave");
            return previousMessage + "You survived the enemy wave.";
        }

        engine.addAction("Encountered " + combatMonsters[combatIndex].getName());
        return previousMessage + "Next enemy: " + combatMonsters[combatIndex].getName() + ".\r\n" + getCombatText();
    }
};

GuiGame::GuiGame() : impl(new GuiGameImpl())
{
}

GuiGame::~GuiGame() = default;

std::string GuiGame::getStatusText() const
{
    return impl->getStatusText();
}

GuiMode GuiGame::getMode() const
{
    return impl->getMode();
}

std::string GuiGame::getCurrentLocation() const
{
    return impl->getCurrentLocation();
}

std::vector<std::string> GuiGame::getAvailablePaths() const
{
    return impl->getAvailablePaths();
}

std::vector<std::pair<std::string, std::string>> GuiGame::getMapPaths() const
{
    return impl->getMapPaths();
}

bool GuiGame::travelToLocation(const std::string& locationName, std::string& message)
{
    return impl->travelToLocation(locationName, message);
}

std::string GuiGame::interact()
{
    return impl->interact();
}

std::string GuiGame::getQuestsText()
{
    return impl->getQuestsText();
}

std::string GuiGame::getInventoryText()
{
    return impl->getInventoryText();
}

std::string GuiGame::getLogText() const
{
    return impl->getLogText();
}

std::string GuiGame::buyHealth()
{
    return impl->buyHealth();
}

std::string GuiGame::buyWeapon()
{
    return impl->buyWeapon();
}

std::string GuiGame::sellWeapon()
{
    return impl->sellWeapon();
}

std::string GuiGame::buyPotion()
{
    return impl->buyPotion();
}

std::string GuiGame::buyArmor()
{
    return impl->buyArmor();
}

std::string GuiGame::leaveStore()
{
    return impl->leaveStore();
}

std::string GuiGame::attackMonster()
{
    return impl->attackMonster();
}

std::string GuiGame::dodgeMonster()
{
    return impl->dodgeMonster();
}

std::string GuiGame::useHealthPotion()
{
    return impl->useHealthPotion();
}

std::string GuiGame::runFromCombat()
{
    return impl->runFromCombat();
}

std::string GuiGame::fightSlime()
{
    return impl->fightSlime();
}

std::string GuiGame::fightFangedBeast()
{
    return impl->fightFangedBeast();
}

std::string GuiGame::leaveCaves()
{
    return impl->leaveCaves();
}

std::string GuiGame::getStoreText() const
{
    return impl->getStoreText();
}

std::string GuiGame::getCombatText() const
{
    return impl->getCombatText();
}


