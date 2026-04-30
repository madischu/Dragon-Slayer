#include "Store.h"
#include <iostream>

Store::Store()
    : Location("Store", "You enter the store. Would you like to make a purchase?")
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
        std::cout << "3: Sell Weapon (+15 gold)" << std::endl;
        std::cout << "4: Leave Store" << std::endl;

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

    if (player.getGold() >= 10 && player.getHealth() <= 300)
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
    std::vector<Weapon>& weapons = player.getWeapons();
    int currentWeaponIndex = player.getCurrentWeaponIndex();

    if (player.getGold() >= 30)
    {
        if (currentWeaponIndex < 3)
        {
            if (currentWeaponIndex == 1 && player.getXP() < 50)
            {
                std::cout << "\nYou need at least 50 XP to purchase the Claw Hammer!" << std::endl;
                return;
            }

            if (currentWeaponIndex == 2 && player.getXP() < 150)
            {
                std::cout << "\nYou need at least 75 XP to purchase the Sword!" << std::endl;
                return;
            }

            player.subtractGold(30);

            if (currentWeaponIndex == 0)
            {
                weapons.push_back(Weapon("Dagger", 30, "sharp"));
            }
            else if (currentWeaponIndex == 1)
            {
                weapons.push_back(Weapon("Claw Hammer", 50, "blunt"));
            }
            else if (currentWeaponIndex == 2)
            {
                weapons.push_back(Weapon("Sword", 100, "sharp"));
            }

            player.setCurrentWeaponIndex(currentWeaponIndex + 1);

            std::cout << "\nYou now have a " << player.getCurrentWeapon().getName()
                      << " that deals " << player.getCurrentWeapon().getPower()
                      << " damage." << std::endl;

            std::cout << "\nIn your inventory you have: ";

            for (int i = 0; i <= player.getCurrentWeaponIndex(); i++)
            {
                std::cout << weapons[i].getName();

                if (i < player.getCurrentWeaponIndex())
                {
                    std::cout << ", ";
                }
            }

            std::cout << ".\n" << std::endl;
        }
        else
        {
            std::cout << "You already have the strongest weapon!" << std::endl;
        }
    }
    else
    {
        std::cout << "\nNot enough gold!" << std::endl;
    }
}

void Store::sellWeapon(Player& player)
{
    std::vector<Weapon>& weapons = player.getWeapons();
    int currentWeaponIndex = player.getCurrentWeaponIndex();

    if (currentWeaponIndex > 0)
    {
        std::string soldWeaponName = weapons[currentWeaponIndex].getName();
        player.addGold(15);
        weapons.erase(weapons.begin() + currentWeaponIndex);
        player.setCurrentWeaponIndex(currentWeaponIndex - 1);

        std::cout << "You sold your " << soldWeaponName << "." << std::endl;
        std::cout << "In your inventory you have: ";

        for (int i = 0; i <= player.getCurrentWeaponIndex(); i++)
        {
            std::cout << weapons[i].getName();

            if (i < player.getCurrentWeaponIndex())
            {
                std::cout << ", ";
            }
        }

        std::cout << "." << std::endl;
    }
    else
    {
        std::cout << "You can't sell your only weapon!" << std::endl;
    }
}
