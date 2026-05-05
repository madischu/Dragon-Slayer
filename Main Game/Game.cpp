#include "Game.h"
#include "ConsoleColor.h"
#include <iostream>
#include <limits>

namespace
{
    bool readMenuChoice(int& choice)
    {
        if (std::cin >> choice)
        {
            return true;
        }

        if (std::cin.eof())
        {
            return false;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number." << std::endl;
        return false;
    }

}

Game::Game()
{
    running = true;
}

void Game::start()
{
    std::cout << "\nWelcome to Dragon Slayer. You must defeat the dragon that is preventing people from leaving the town.\n\nYou are in the Town Square. You see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'." << std::endl;

    while (running)
    {
        displayMainMenu();

        int choice;
        if (!readMenuChoice(choice))
        {
            if (std::cin.eof())
            {
                running = false;
            }

            continue;
        }

        if (choice == 1)
        {
            travel();
        }
        else if (choice == 2)
        {
            engine.addAction("Viewed stats");
            engine.getPlayer().displayStats();
        }
        else if (choice == 3)
        {
            engine.addAction("Viewed inventory");
            std::cout << engine.getInventoryText() << std::endl;
        }
        else if (choice == 4)
        {
            std::cout << engine.getLogText() << std::endl;
        }
        else if (choice == 5)
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
    std::cout << "\nChoose an option:" << std::endl;
    std::cout << "1: Travel" << std::endl;
    std::cout << "2: View Stats" << std::endl;
    std::cout << "3: View Inventory" << std::endl;
    std::cout << "4: View Action Log" << std::endl;
    std::cout << "5: Quit Game" << std::endl;
}

void Game::travel()
{
    engine.getWorldMap().displayAvailablePaths();

    std::cout << "\nWhere would you like to go? ";
    int choice;
    if (!readMenuChoice(choice))
    {
        return;
    }

    std::string message;
    if (engine.travelToPathIndex(choice, message))
    {
        if (engine.getCurrentLocation() == "Dark Forest")
        {
            ConsoleColor::printLine(message, ConsoleColor::Color::DarkGreen);
        }
        else
        {
            std::cout << message << std::endl;
        }

        processLocation();
    }
}

void Game::processLocation()
{
    std::string location = engine.getCurrentLocation();

    if (location == "Town Square")
    {
        std::cout << "\nYou are in the Town Square. You see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'." << std::endl;
        engine.addAction("Visited Town Square");
    }
    else if (location == "Store")
    {
        engine.addAction("Visited Store");
        store.enter(engine.getPlayer(), engine.getActionLog());
    }
    else if (location == "Caves")
    {
        caves.enter(engine);

        if (engine.getPlayer().getHealth() <= 0)
        {
            running = false;
        }
    }
    else if (location == "Dark Forest")
    {
        engine.enterDarkForest();
    }
    else if (location == "Ancient Ruins")
    {
        ancientRuins.enter(engine);
    }
    else if (location == "Dragon's Lair")
    {
        engine.addAction("Entered the Dragon's Lair");
        if (!dragonsLair.enter(engine.getPlayer()) && engine.getPlayer().getHealth() <= 0)
        {
            engine.addAction("Player was defeated");
            running = false;
        }
    }
}
