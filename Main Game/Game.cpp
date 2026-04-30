#include "Game.h"
#include <iostream>

Game::Game()
{
    running = true;
    worldMap.buildDefaultMap();
}

void Game::start()
{
    std::cout << "\nWelcome to Dragon Slayer Deluxe!" << std::endl;

    while (running)
    {
        displayMainMenu();

        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
            travel();
        }
        else if (choice == 2)
        {
            processLocation();
        }
        else if (choice == 3)
        {
            player.displayStats();
        }
        else if (choice == 4)
        {
            player.displayInventory();
        }
        else if (choice == 5)
        {
            actionLog.displayRecentActions();
        }
        else if (choice == 6)
        {
            std::cout << "\nExiting the game. Thanks for playing!" << std::endl;
            running = false;
        }
        else
        {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void Game::displayMainMenu()
{
    worldMap.displayCurrentLocation();

    std::cout << "\nChoose an option:" << std::endl;
    std::cout << "1: Travel" << std::endl;
    std::cout << "2: Interact with Current Location" << std::endl;
    std::cout << "3: View Stats" << std::endl;
    std::cout << "4: View Inventory" << std::endl;
    std::cout << "5: View Action Log" << std::endl;
    std::cout << "6: Quit Game" << std::endl;
}

void Game::travel()
{
    worldMap.displayAvailablePaths();

    std::cout << "\nWhere would you like to go? ";
    int choice;
    std::cin >> choice;

    std::string previousLocation = worldMap.getCurrentLocation();

    if (worldMap.moveToLocation(choice))
    {
        std::string newLocation = worldMap.getCurrentLocation();
        actionLog.push("Moved from " + previousLocation + " to " + newLocation);
    }
}

void Game::processLocation()
{
    std::string location = worldMap.getCurrentLocation();

    if (location == "Town Square")
    {
        std::cout << "\nYou are standing in the Town Square." << std::endl;
        actionLog.push("Visited Town Square");
    }
    else if (location == "Store")
    {
        store.enter(player);
        actionLog.push("Visited Store");
    }
    else if (location == "Caves")
    {
        EnemyQueue enemies;
        enemies.buildCaveWave();

        actionLog.push("Started a cave enemy wave");
        fightEnemyWave(enemies);
    }
    else if (location == "Dark Forest")
    {
        EnemyQueue enemies;
        enemies.buildForestWave();

        actionLog.push("Started a forest enemy wave");
        fightEnemyWave(enemies);
    }
    else if (location == "Ancient Ruins")
    {
        EnemyQueue enemies;
        enemies.buildRuinsWave();

        actionLog.push("Started a ruins enemy wave");
        fightEnemyWave(enemies);
    }
    else if (location == "Dragon's Lair")
    {
        Monster dragon("Dragon", 35, 300, "sharp");

        actionLog.push("Entered the Dragon's Lair");
        fightSingleMonster(dragon);
    }
}

void Game::fightEnemyWave(EnemyQueue& enemies)
{
    std::cout << "\nEnemy wave started!" << std::endl;

    while (!enemies.isEmpty() && player.getHealth() > 0)
    {
        Monster currentMonster = enemies.dequeue();

        std::cout << "\nNext enemy: " << currentMonster.getName() << std::endl;
        actionLog.push("Encountered " + currentMonster.getName());

        fightSingleMonster(currentMonster);

        if (player.getHealth() <= 0)
        {
            actionLog.push("Player was defeated");
            return;
        }
    }

    if (player.getHealth() > 0)
    {
        std::cout << "\nYou survived the enemy wave!" << std::endl;
        actionLog.push("Completed an enemy wave");
    }
}

void Game::fightSingleMonster(Monster& monster)
{
    if (!player.canFightMonster(monster))
    {
        actionLog.push("Could not fight " + monster.getName() + " because XP was too low");
        return;
    }

    while (player.getHealth() > 0 && monster.getHealth() > 0)
    {
        std::cout << "\nYou are fighting a " << monster.getName() << "." << std::endl;
        std::cout << "Monster Health: " << monster.getHealth() << std::endl;

        std::cout << "\nChoose an action:" << std::endl;
        std::cout << "1: Attack" << std::endl;
        std::cout << "2: Dodge" << std::endl;
        std::cout << "3: Use Health Potion" << std::endl;
        std::cout << "4: Run Away" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
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

            if (player.isMonsterHit())
            {
                monster.takeDamage(finalDamage, currentWeapon.getType());

                std::cout << "\nYou attacked with "
                          << currentWeapon.getName()
                          << " and dealt "
                          << finalDamage
                          << " damage."
                          << std::endl;

                actionLog.push("Attacked " + monster.getName() + " with " + currentWeapon.getName());
            }
            else
            {
                std::cout << "\nYou missed!" << std::endl;
                actionLog.push("Missed an attack against " + monster.getName());
            }

            if (monster.getHealth() <= 0)
            {
                player.defeatMonster(monster);
                actionLog.push("Defeated " + monster.getName());
                return;
            }

            int damage = monster.attack();
            player.takeDamage(damage);

            std::cout << "\nYou took " << damage
                      << " damage. Current health: "
                      << player.getHealth()
                      << std::endl;

            actionLog.push(monster.getName() + " attacked the player");
        }
        else if (choice == 2)
        {
            if (player.dodge(monster))
            {
                actionLog.push("Dodged " + monster.getName());
            }
            else
            {
                int damage = monster.attack();
                player.takeDamage(damage);

                std::cout << "\nYou took " << damage
                          << " damage. Current health: "
                          << player.getHealth()
                          << std::endl;

                actionLog.push("Failed to dodge " + monster.getName());
            }
        }
        else if (choice == 3)
        {
            player.usePotion("Health Potion");
            actionLog.push("Used Health Potion");
        }
        else if (choice == 4)
        {
            std::cout << "\nYou ran away from the battle." << std::endl;
            actionLog.push("Ran away from " + monster.getName());
            return;
        }
        else
        {
            std::cout << "Invalid choice." << std::endl;
        }
    }

    if (player.getHealth() <= 0)
    {
        std::cout << "\nYou have been defeated." << std::endl;
        actionLog.push("Player lost the game");
        running = false;
    }
}