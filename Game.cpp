#include "Game.h"
#include <iostream>

Game::Game()
    : running(true)
{
}

void Game::start()
{
    std::cout << "\nWelcome to Dragon Slayer!" << std::endl;

    while (running)
    {
        showMainMenu();
    }
}

void Game::showMainMenu()
{
    if (player.getHealth() <= 0)
    {
        loseGame();
        return;
    }

    std::cout << "\nYou are in the Town Square. You see a sign pointing left that says 'Store' and a sign pointing right that says 'Caves'. \n" << std::endl;
    std::cout << "Choose an option: " << std::endl;
    std::cout << "1: Go to Store" << std::endl;
    std::cout << "2: Go to Caves" << std::endl;
    std::cout << "3: Quit Game" << std::endl;

    int choice;
    std::cin >> choice;

    if (choice == 1)
    {
        store.enter(player);
    }
    else if (choice == 2)
    {
        int caveResult = caves.enter(player);

        if (caveResult == 2)
        {
            bool dragonDefeated = dragonsLair.enter(player);

            if (dragonDefeated)
            {
                std::cout << "You have slain the dragon!" << std::endl;
                winGame();
            }
            else if (player.getHealth() <= 0)
            {
                std::cout << "You have been defeated!" << std::endl;
                loseGame();
            }
        }
        else if (player.getHealth() <= 0)
        {
            std::cout << "You have been defeated!" << std::endl;
            loseGame();
        }
    }
    else if (choice == 3)
    {
        std::cout << "\nExiting the game. Thanks for playing!\n" << std::endl;
        running = false;
    }
    else
    {
        std::cout << "Invalid choice!" << std::endl;
    }
}

void Game::restart()
{
    player = Player();
}

void Game::winGame()
{
    std::cout << "You won the game! Would you like to play again? (y/n)" << std::endl;

    char playAgain;
    std::cin >> playAgain;

    if (playAgain == 'y' || playAgain == 'Y')
    {
        restart();
    }
    else
    {
        std::cout << "Thanks for playing!" << std::endl;
        running = false;
    }
}

void Game::loseGame()
{
    std::cout << "You lost the game. Would you like to play again? (y/n)" << std::endl;

    char playAgain;
    std::cin >> playAgain;

    if (playAgain == 'y' || playAgain == 'Y')
    {
        restart();
    }
    else
    {
        std::cout << "\nExiting the game. Thanks for playing!\n" << std::endl;
        running = false;
    }
}
