#include "Game.h"
#include "ConsoleColor.h"
#include "ConsoleInput.h"
#include <iostream>

Game::Game()
{
    running = true;
}

void Game::start()
{
    displayIntro();

    while (running)
    {
        displayMainMenu();

        int choice;
        if (!ConsoleInput::readInt(choice))
        {
            running = false;
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
            ConsoleColor::printLine("\nExiting the game. Thanks for playing!", ConsoleColor::Color::LightPurple);
            running = false;
        }
        else
        {
            ConsoleInput::printInvalidInput();
        }
    }
}

void Game::displayIntro()
{
    ConsoleColor::printLine("\nWelcome to Dragon Slayer. You must defeat the dragon that is preventing people from leaving the town.", ConsoleColor::Color::LightPurple);
    ConsoleColor::printLine("\nYou are in the Town Square. You see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'.", ConsoleColor::Color::DarkMagenta);
}

void Game::displayMainMenu()
{
    ConsoleColor::printLine("\nChoose an option:", ConsoleColor::Color::DarkMagenta);
    std::cout << "1: Travel" << std::endl;
    std::cout << "2: View Stats" << std::endl;
    std::cout << "3: View Inventory" << std::endl;
    std::cout << "4: View Action Log" << std::endl;
    std::cout << "5: Quit Game" << std::endl;
}

void Game::travel()
{
    engine.getWorldMap().displayAvailablePaths();

    ConsoleColor::print("\nWhere would you like to go? ", ConsoleColor::Color::DarkMagenta);
    // Travel stays in this loop until the player chooses a valid connected path.
    while (running)
    {
        int choice;
        if (!ConsoleInput::readInt(choice))
        {
            running = false;
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
            return;
        }

        if (!message.empty())
        {
            ConsoleInput::printInvalidInput();
        }

        ConsoleColor::print("\nWhere would you like to go? ", ConsoleColor::Color::Magenta);
    }
}

void Game::processLocation()
{
    std::string location = engine.getCurrentLocation();

    if (location == "Town Square")
    {
        ConsoleColor::printLine("\nYou are in the Town Square. You look around and see four signs marked 'Store', 'Ancient Ruins', 'Dark Forest', and 'Caves'. What would you like to do?", ConsoleColor::Color::DarkMagenta);
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
            handlePlayerDefeat();
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
            handlePlayerDefeat();
        }
    }
}

void Game::handlePlayerDefeat()
{
    ConsoleColor::printLine("\nYou have been defeated!", ConsoleColor::Color::Red);

    // Replay resets the game world and player, but keeps the linked-list action history intact.
    while (true)
    {
        ConsoleColor::printLine("\nPlay Again?", ConsoleColor::Color::DarkMagenta);
        std::cout << "1: Replay" << std::endl;
        std::cout << "2: Quit Game" << std::endl;

        int choice;
        if (!ConsoleInput::readInt(choice))
        {
            running = false;
            return;
        }

        if (choice == 1)
        {
            engine.restartPreservingActionLog();
            engine.addAction("Replayed after defeat");
            running = true;
            displayIntro();
            return;
        }

        if (choice == 2)
        {
            ConsoleColor::printLine("\nExiting the game. Thanks for playing!", ConsoleColor::Color::LightPurple);
            running = false;
            return;
        }

        ConsoleInput::printInvalidInput();
    }
}
