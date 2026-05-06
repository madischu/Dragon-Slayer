#include "Store.h"
#include "../Main Game/ConsoleColor.h"
#include "../Main Game/ConsoleInput.h"
#include <iostream>
#include <string>

Store::Store()
{
}

namespace
{
    void printStoreLine(const std::string& message)
    {
        ConsoleColor::printLine(message, ConsoleColor::Color::LightBrown);
    }
}

void Store::enter(Player& player, ActionStack& actionLog)
{
    printStoreLine("\nYou enter the store.\n");

    while (true)
    {
        player.displayStats();

        ConsoleColor::printLine("\nWhat would you like to do?\n", ConsoleColor::Color::LightBrown);

        std::cout << "1: Buy Health (+10 HP for -10 gold)" << std::endl;
        std::cout << "2: Buy Weapon (-30 gold)" << std::endl;
        std::cout << "3: Sell Equipped Weapon (+15 gold)" << std::endl;
        std::cout << "4: Buy Potion (-15 gold)" << std::endl;
        std::cout << "5: Buy Armor (-40 gold)" << std::endl;
        std::cout << "6: View Inventory" << std::endl;
        std::cout << "7: Use Health Potion" << std::endl;
        std::cout << "8: Leave Store" << std::endl;

        int choice;
        if (!ConsoleInput::readInt(choice))
        {
            return;
        }

        if (choice == 1)
        {
            buyHealth(player, actionLog);
        }
        else if (choice == 2)
        {
            buyWeapon(player, actionLog);
        }
        else if (choice == 3)
        {
            sellWeapon(player, actionLog);
        }
        else if (choice == 4)
        {
            buyPotion(player, actionLog);
        }
        else if (choice == 5)
        {
            buyArmor(player, actionLog);
        }
        else if (choice == 6)
        {
            player.displayInventory();
            actionLog.push("Viewed inventory in Store");
        }
        else if (choice == 7)
        {
            bool hasPotion = player.getInventory().containsItem("Health Potion");
            player.usePotion("Health Potion");
            actionLog.push(hasPotion ? "Used Health Potion in Store" : "Could not use Health Potion in Store");
        }
        else if (choice == 8)
        {
            printStoreLine("\nLeaving the store...");
            actionLog.push("Left Store");
            return;
        }
        else
        {
            ConsoleInput::printInvalidInput();
            actionLog.push("Entered invalid Store choice");
        }
    }
}

void Store::buyHealth(Player& player, ActionStack& actionLog)
{
    if (player.getHealth() == 300)
    {
        printStoreLine("Your health is already at the maximum!");
        actionLog.push("Could not buy health because health was full");
        return;
    }

    if (player.getGold() >= 10)
    {
        player.subtractGold(10);
        player.addHealth(10);
        printStoreLine("\nYou bought 10 health.\n");
        actionLog.push("Bought 10 health");
    }
    else
    {
        printStoreLine("Not enough gold!");
        actionLog.push("Could not buy health because gold was too low");
    }
}

void Store::buyWeapon(Player& player, ActionStack& actionLog)
{
    Inventory& inventory = player.getInventory();

    if (player.getGold() < 30)
    {
        printStoreLine("\nNot enough gold!");
        actionLog.push("Could not buy weapon because gold was too low");
        return;
    }

    if (!inventory.containsItem("Dagger"))
    {
        player.subtractGold(30);
        inventory.addItem(Item("Dagger", ItemType::Weapon, 30, "Sharp starter weapon"));
        player.equipWeapon("Dagger");
        actionLog.push("Bought and equipped Dagger");
    }
    else if (!inventory.containsItem("Claw Hammer"))
    {
        if (player.getXP() < 50)
        {
            printStoreLine("\nYou need at least 50 XP to purchase the Claw Hammer!");
            actionLog.push("Could not buy Claw Hammer because XP was too low");
            return;
        }

        player.subtractGold(30);
        inventory.addItem(Item("Claw Hammer", ItemType::Weapon, 50, "Strong blunt weapon"));
        player.equipWeapon("Claw Hammer");
        actionLog.push("Bought and equipped Claw Hammer");
    }
    else if (!inventory.containsItem("Sword"))
    {
        if (player.getXP() < 150)
        {
            printStoreLine("\nYou need at least 150 XP to purchase the Sword!");
            actionLog.push("Could not buy Sword because XP was too low");
            return;
        }

        player.subtractGold(30);
        inventory.addItem(Item("Sword", ItemType::Weapon, 100, "Required to defeat the Dragon"));
        player.equipWeapon("Sword");
        actionLog.push("Bought and equipped Sword");
    }
    else
    {
        printStoreLine("You already have the strongest weapon!");
        actionLog.push("Could not buy weapon because strongest weapon was already owned");
        return;
    }

    inventory.displayInventory();
}

void Store::sellWeapon(Player& player, ActionStack& actionLog)
{
    Inventory& inventory = player.getInventory();
    Weapon currentWeapon = player.getCurrentWeapon();

    if (currentWeapon.getName() == "Stick")
    {
        ConsoleColor::printLine("\nYou can't sell your basic weapon!\n", ConsoleColor::Color::Red);
        actionLog.push("Could not sell basic weapon");
        return;
    }

    if (inventory.removeItem(currentWeapon.getName()))
    {
        player.addGold(15);
        printStoreLine("You sold your " + currentWeapon.getName() + ".");
        player.equipWeapon("Stick");
        inventory.displayInventory();
        actionLog.push("Sold " + currentWeapon.getName());
    }
    else
    {
        printStoreLine("Could not sell weapon.");
        actionLog.push("Could not sell " + currentWeapon.getName());
    }
}

void Store::buyPotion(Player& player, ActionStack& actionLog)
{
    if (player.getGold() < 15)
    {
        printStoreLine("\nNot enough gold!");
        actionLog.push("Could not buy Health Potion because gold was too low");
        return;
    }

    player.subtractGold(15);
    player.addItemToInventory(Item("Health Potion", ItemType::Potion, 50, "Restores 50 health"));

    printStoreLine("\nYou bought a Health Potion.");
    actionLog.push("Bought Health Potion");
}

void Store::buyArmor(Player& player, ActionStack& actionLog)
{
    Inventory& inventory = player.getInventory();

    if (inventory.containsItem("Leather Armor"))
    {
        printStoreLine("You already own Leather Armor.");
        actionLog.push("Could not buy Leather Armor because it was already owned");
        return;
    }

    if (player.getGold() < 40)
    {
        printStoreLine("\nNot enough gold!");
        actionLog.push("Could not buy Leather Armor because gold was too low");
        return;
    }

    player.subtractGold(40);
    player.addItemToInventory(Item("Leather Armor", ItemType::Armor, 10, "Basic armor"));

    printStoreLine("\nYou bought Leather Armor.");
    actionLog.push("Bought Leather Armor");
}
