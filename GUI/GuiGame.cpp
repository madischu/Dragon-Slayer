#include "GuiGame.h"

#include "ActionStack.h"
#include "Monster.h"
#include "Player.h"
#include "WorldMap.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

class GuiGameImpl
{
public:
    GuiGameImpl()
    {
        worldMap.buildDefaultMap();
        addAction("Started a new game");
    }

    std::string getStatusText() const
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

    GuiMode getMode() const
    {
        return mode;
    }

    std::string getCurrentLocation() const
    {
        return worldMap.getCurrentLocation();
    }

    std::vector<std::string> getAvailablePaths() const
    {
        return worldMap.getAvailablePaths();
    }

    std::vector<std::pair<std::string, std::string>> getMapPaths() const
    {
        return worldMap.getAllPaths();
    }

    bool travelToLocation(const std::string& locationName, std::string& message)
    {
        std::vector<std::string> availablePaths = worldMap.getAvailablePaths();

        for (int i = 0; i < static_cast<int>(availablePaths.size()); ++i)
        {
            if (availablePaths[i] == locationName)
            {
                std::string previousLocation = worldMap.getCurrentLocation();
                worldMap.moveToLocation(i + 1);
                std::string newLocation = worldMap.getCurrentLocation();

                addAction("Moved from " + previousLocation + " to " + newLocation);
                message = "Traveled to " + newLocation + ".";
                return true;
            }
        }

        if (locationName == worldMap.getCurrentLocation())
        {
            message = "You are already at " + locationName + ".";
            return false;
        }

        message = "You cannot travel directly to " + locationName + " from here.";
        return false;
    }

    std::string interact()
    {
        std::string location = worldMap.getCurrentLocation();

        if (location == "Town Square")
        {
            addAction("Visited Town Square");
            return "You are in the Town Square. You see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'.";
        }

        if (location == "Store")
        {
            mode = GuiMode::Store;
            addAction("Visited Store");
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
            std::vector<Monster> monsters =
            {
                Monster("Slime", 2, 15, "blunt"),
                Monster("Fanged Beast", 8, 60, "sharp")
            };

            return startCombat(monsters, "Started a cave enemy wave");
        }

        if (location == "Dark Forest")
        {
            std::vector<Monster> monsters =
            {
                Monster("Slime", 2, 15, "blunt"),
                Monster("Fanged Beast", 8, 60, "sharp"),
                Monster("Ghoul", 20, 150, "blunt")
            };

            return startCombat(monsters, "Started a forest enemy wave");
        }

        if (location == "Ancient Ruins")
        {
            std::vector<Monster> monsters =
            {
                Monster("Fanged Beast", 8, 60, "sharp"),
                Monster("Ghoul", 20, 150, "blunt")
            };

            return startCombat(monsters, "Started a ruins enemy wave");
        }

        addAction("Explored " + location + " from GUI");
        return "There is nothing else to do here right now.";
    }

    std::string getQuestsText()
    {
        return "View Quests Here";
    }

    std::string getInventoryText()
    {
        std::ostringstream output;
        output << "Inventory\r\n";

        std::vector<std::string> knownItems =
        {
            "Stick",
            "Dagger",
            "Claw Hammer",
            "Sword",
            "Health Potion",
            "Leather Armor",
            "Slime Gel",
            "Beast Fang",
            "Ghoul Bone",
            "Dragon Scale"
        };

        bool foundItem = false;

        for (const std::string& itemName : knownItems)
        {
            if (player.getInventory().containsItem(itemName))
            {
                output << "- " << itemName << "\r\n";
                foundItem = true;
            }
        }

        if (!foundItem)
        {
            output << "No items.";
        }

        return output.str();
    }

    std::string buyHealth()
    {
        if (player.getHealth() == 300)
        {
            return getStoreText("Your health is already at the maximum.");
        }

        if (player.getGold() < 10)
        {
            return getStoreText("Not enough gold.");
        }

        player.subtractGold(10);
        player.addHealth(10);
        addAction("Bought 10 health");
        return getStoreText("You bought 10 health.");
    }

    std::string buyWeapon()
    {
        Inventory& inventory = player.getInventory();

        if (player.getGold() < 30)
        {
            return getStoreText("Not enough gold.");
        }

        if (!inventory.containsItem("Dagger"))
        {
            player.subtractGold(30);
            inventory.addItem(Item("Dagger", ItemType::Weapon, 30, "Sharp starter weapon"));
            player.equipWeapon("Dagger");
            addAction("Bought Dagger");
            return getStoreText("You bought and equipped the Dagger.");
        }

        if (!inventory.containsItem("Claw Hammer"))
        {
                if (player.getXP() < 50)
                {
                    return getStoreText("You need at least 50 XP to purchase the Claw Hammer.");
                }

            player.subtractGold(30);
            inventory.addItem(Item("Claw Hammer", ItemType::Weapon, 50, "Strong blunt weapon"));
                player.equipWeapon("Claw Hammer");
                addAction("Bought Claw Hammer");
                return getStoreText("You bought and equipped the Claw Hammer.");
            }

        if (!inventory.containsItem("Sword"))
        {
                if (player.getXP() < 150)
                {
                    return getStoreText("You need at least 150 XP to purchase the Sword.");
                }

            player.subtractGold(30);
            inventory.addItem(Item("Sword", ItemType::Weapon, 100, "Required to defeat the Dragon"));
                player.equipWeapon("Sword");
                addAction("Bought Sword");
                return getStoreText("You bought and equipped the Sword.");
            }

        return getStoreText("You already have the strongest weapon.");
    }

    std::string sellWeapon()
    {
        Inventory& inventory = player.getInventory();
        Weapon currentWeapon = player.getCurrentWeapon();

        if (currentWeapon.getName() == "Stick")
        {
            return getStoreText("You can't sell your basic weapon.");
        }

        if (!inventory.removeItem(currentWeapon.getName()))
        {
            return getStoreText("Could not sell weapon.");
        }

        player.addGold(15);
        player.equipWeapon("Stick");
        addAction("Sold " + currentWeapon.getName());
        return getStoreText("You sold your " + currentWeapon.getName() + ".");
    }

    std::string buyPotion()
    {
        if (player.getGold() < 15)
        {
            return getStoreText("Not enough gold.");
        }

        player.subtractGold(15);
        player.addItemToInventory(Item("Health Potion", ItemType::Potion, 50, "Restores 50 health"));
        addAction("Bought Health Potion");
        return getStoreText("You bought a Health Potion.");
    }

    std::string buyArmor()
    {
        Inventory& inventory = player.getInventory();

        if (inventory.containsItem("Leather Armor"))
        {
            return getStoreText("You already own Leather Armor.");
        }

        if (player.getGold() < 40)
        {
            return getStoreText("Not enough gold.");
        }

        player.subtractGold(40);
        player.addItemToInventory(Item("Leather Armor", ItemType::Armor, 10, "Basic armor"));
        addAction("Bought Leather Armor");
        return getStoreText("You bought Leather Armor.");
    }

    std::string leaveStore()
    {
        mode = GuiMode::Main;
        addAction("Left Store");
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

        if (!player.canFightMonster(monster))
        {
            addAction("Could not fight " + monster.getName() + " because XP was too low");
            mode = GuiMode::Main;
            return "You are not strong enough to fight the " + monster.getName() + ".";
        }

        Weapon currentWeapon = player.getCurrentWeapon();
        int bonus = player.getXP() / 10;
        int baseDamage = currentWeapon.getPower();
        int totalDamage = baseDamage + bonus;
        int effectiveness = player.getWeaponEffectiveness(currentWeapon, monster);
        int finalDamage = 0;

        if (baseDamage > 0)
        {
            finalDamage = (totalDamage * effectiveness) / baseDamage;
        }

        std::ostringstream output;
        output << "You are fighting a " << monster.getName() << ".\r\n";

        if (player.isMonsterHit())
        {
            monster.takeDamage(finalDamage, currentWeapon.getType());
            output << "You attacked with " << currentWeapon.getName()
                   << " and dealt " << finalDamage << " damage.\r\n";
            addAction("Attacked " + monster.getName() + " with " + currentWeapon.getName());
        }
        else
        {
            output << "You missed.\r\n";
            addAction("Missed an attack against " + monster.getName());
        }

        if (monster.getHealth() <= 0)
        {
            player.defeatMonster(monster);
            addAction("Defeated " + monster.getName());
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

        if (player.dodge(monster))
        {
            addAction("Dodged " + monster.getName());
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
        if (!player.getInventory().containsItem("Health Potion"))
        {
            return "You do not have that potion.";
        }

        player.usePotion("Health Potion");
        addAction("Used Health Potion");

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
        addAction("Ran away from " + monsterName);
        return "You ran away from the battle.";
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

    std::string getLogText() const
    {
        std::ostringstream output;
        output << "Recent Actions\r\n";

        for (int i = static_cast<int>(actions.size()) - 1; i >= 0; --i)
        {
            output << "- " << actions[i] << "\r\n";
        }

        return output.str();
    }

private:
    Player player;
    WorldMap worldMap;
    ActionStack actionLog;
    std::vector<std::string> actions;
    GuiMode mode = GuiMode::Main;
    std::vector<Monster> combatMonsters;
    int combatIndex = 0;

    void addAction(const std::string& action)
    {
        actionLog.push(action);
        actions.push_back(action);
    }

    std::string getStoreText(const std::string& message) const
    {
        return message + "\r\n\r\n" + getStoreText();
    }

    std::string startCombat(const std::vector<Monster>& monsters, const std::string& action)
    {
        combatMonsters = monsters;
        combatIndex = 0;
        mode = GuiMode::Combat;
        addAction(action);

        if (combatMonsters.empty())
        {
            mode = GuiMode::Main;
            return "There are no enemies here.";
        }

        addAction("Encountered " + combatMonsters[combatIndex].getName());
        return "Enemy wave started.\r\n" + getCombatText();
    }

    std::string monsterAttack(Monster& monster)
    {
        int damage = monster.attack();
        player.takeDamage(damage);
        addAction(monster.getName() + " attacked the player");

        std::ostringstream output;
        output << "The " << monster.getName() << " attacked for " << damage
               << " damage. Current health: " << player.getHealth() << ".";

        if (player.getHealth() <= 0)
        {
            mode = GuiMode::Main;
            combatMonsters.clear();
            combatIndex = 0;
            addAction("Player lost the game");
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
            addAction("Completed an enemy wave");
            return previousMessage + "You survived the enemy wave.";
        }

        addAction("Encountered " + combatMonsters[combatIndex].getName());
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

std::string GuiGame::getStoreText() const
{
    return impl->getStoreText();
}

std::string GuiGame::getCombatText() const
{
    return impl->getCombatText();
}
