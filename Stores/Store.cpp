#include "Store.h"
#include <iostream>

Store::Store() : Location("Store", "You enter the store. Would you like to make a purchase?")
{
}

void Store::enter(Player& player)
{
    Location::enter();

    std::cout << "\nYou enter the store.\n" << std::endl;

    while (true)
    {
        player.displayStats();

        std::cout << "\nWhat would you like to do?\n" << std::endl;
        std::cout << "1: Buy Health (+10 HP for -10 gold)" << std::endl;
        std::cout << "2: Buy Weapon (-30 gold)" << std::endl;
        std::cout << "3: Sell Equipped Weapon (+15 gold)" << std::endl;
        std::cout << "4: Buy Potion (-15 gold)" << std::endl;
        std::cout << "5: Buy Armor (-40 gold)" << std::endl;
        std::cout << "6: View Inventory" << std::endl;
        std::cout << "7: Use Health Potion" << std::endl;
        std::cout << "8: Leave Store" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
            buyHealth(player);
        }
        else if (choice == 2)
        {
            buyWeapon(player);
        }
        else if (choice == 3)
        {
            sellWeapon(player);
        }
        else if (choice == 4)
        {
            buyPotion(player);
        }
        else if (choice == 5)
        {
            buyArmor(player);
        }
        else if (choice == 6)
        {
            player.displayInventory();
        }
        else if (choice == 7)
        {
            player.usePotion("Health Potion");
        }
        else if (choice == 8)
        {
            std::cout << "\nLeaving the store..." << std::endl;
            return;
        }
        else
        {
            std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void Store::buyHealth(Player& player)
{
    if (player.getHealth() == 300)
    {
        std::cout << "Your health is already at the maximum!" << std::endl;
        return;
    }

    if (player.getGold() >= 10)
    {
        player.subtractGold(10);
        player.addHealth(10);
        std::cout << "\nYou bought 10 health.\n" << std::endl;
    }
    else
    {
        std::cout << "Not enough gold!" << std::endl;
    }
}

void Store::buyWeapon(Player& player)
{
    Inventory& inventory = player.getInventory();

    if (player.getGold() < 30)
    {
        std::cout << "\nNot enough gold!" << std::endl;
        return;
    }

    if (!inventory.containsItem("Dagger"))
    {
        player.subtractGold(30);
        inventory.addItem(Item("Dagger", ItemType::Weapon, 30, "Sharp starter weapon"));
        player.equipWeapon("Dagger");
    }
    else if (!inventory.containsItem("Claw Hammer"))
    {
        if (player.getXP() < 50)
        {
            std::cout << "\nYou need at least 50 XP to purchase the Claw Hammer!" << std::endl;
            return;
        }

        player.subtractGold(30);
        inventory.addItem(Item("Claw Hammer", ItemType::Weapon, 50, "Strong blunt weapon"));
        player.equipWeapon("Claw Hammer");
    }
    else if (!inventory.containsItem("Sword"))
    {
        if (player.getXP() < 150)
        {
            std::cout << "\nYou need at least 150 XP to purchase the Sword!" << std::endl;
            return;
        }

        player.subtractGold(30);
        inventory.addItem(Item("Sword", ItemType::Weapon, 100, "Required to defeat the Dragon"));
        player.equipWeapon("Sword");
    }
    else
    {
        std::cout << "You already have the strongest weapon!" << std::endl;
        return;
    }

    inventory.displayInventory();
}

void Store::sellWeapon(Player& player)
{
    Inventory& inventory = player.getInventory();
    Weapon currentWeapon = player.getCurrentWeapon();

    if (currentWeapon.getName() == "Stick")
    {
        std::cout << "You can't sell your basic weapon!" << std::endl;
        return;
    }

    if (inventory.removeItem(currentWeapon.getName()))
    {
        player.addGold(15);
        std::cout << "You sold your " << currentWeapon.getName() << "." << std::endl;
        player.equipWeapon("Stick");
        inventory.displayInventory();
    }
    else
    {
        std::cout << "Could not sell weapon." << std::endl;
    }
}

void Store::buyPotion(Player& player)
{
    if (player.getGold() < 15)
    {
        std::cout << "\nNot enough gold!" << std::endl;
        return;
    }

    player.subtractGold(15);
    player.addItemToInventory(Item("Health Potion", ItemType::Potion, 50, "Restores 50 health"));

    std::cout << "\nYou bought a Health Potion." << std::endl;
}

void Store::buyArmor(Player& player)
{
    Inventory& inventory = player.getInventory();

    if (inventory.containsItem("Leather Armor"))
    {
        std::cout << "You already own Leather Armor." << std::endl;
        return;
    }

    if (player.getGold() < 40)
    {
        std::cout << "\nNot enough gold!" << std::endl;
        return;
    }

    player.subtractGold(40);
    player.addItemToInventory(Item("Leather Armor", ItemType::Armor, 10, "Basic armor"));

    std::cout << "\nYou bought Leather Armor." << std::endl;
}