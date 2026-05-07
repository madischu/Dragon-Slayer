#include "Caves.h"
#include "CombatSystem.h"
#include "../../Main Game/ConsoleColor.h"
#include "../../Main Game/ConsoleInput.h"
#include "EnemyQueue.h"
#include <cstdlib>
#include <iostream>
#include <vector>

Caves::Caves()
{
}

int Caves::enter(GameEngine& engine)
{
    Player& player = engine.getPlayer();

    ConsoleColor::printLine("\nYou enter a cave. You see some monsters.\n", ConsoleColor::Color::Blue);
    engine.addAction("Entered the caves");

    if ((rand() % 100) < 25)
    {
        engine.addAction("Started a cave enemy wave");

        EnemyQueue enemies;
        enemies.buildCaveWaveForWeapon(player.getCurrentWeapon());

        // Enemy waves are resolved here as one sequence so the cave does not restart between enemies.
        while (!enemies.isEmpty() && player.getHealth() > 0)
        {
            Monster currentMonster = enemies.dequeue();
            engine.addAction("Encountered " + currentMonster.getName());

            if (!player.canFightMonster(currentMonster))
            {
                ConsoleColor::printLine("You are not strong enough to fight this monster!", ConsoleColor::Color::DarkCyan);
                engine.addAction("Could not fight " + currentMonster.getName() + " because XP was too low");
                return 0;
            }

            if (!CombatSystem::fight(player, currentMonster, true, false, true))
            {
                if (player.getHealth() > 0)
                {
                    engine.addAction("Ran away from " + currentMonster.getName());
                }
                else
                {
                    engine.addAction("Player was defeated");
                }

                return 0;
            }

            engine.addAction("Defeated " + currentMonster.getName());
        }

        if (player.getHealth() > 0)
        {
            ConsoleColor::printLine("\nYou survived the enemy wave!", ConsoleColor::Color::Blue);
            ConsoleColor::printLine("\nLeaving the caves...", ConsoleColor::Color::Blue);
            engine.addAction("Completed a cave enemy wave");
        }

        return 0;
    }

    EnemyQueue enemies;
    enemies.buildCaveWave();

    std::vector<Monster> monsters;
    // Convert the queue into a stable menu list so the player can choose exactly one monster.
    while (!enemies.isEmpty())
    {
        monsters.push_back(enemies.dequeue());
    }

    ConsoleColor::printLine("Choose a monster to fight:", ConsoleColor::Color::Blue);
    for (int i = 0; i < static_cast<int>(monsters.size()); ++i)
    {
        std::cout << i + 1 << ": " << monsters[i].getName() << std::endl;
    }
    std::cout << monsters.size() + 1 << ": Leave" << std::endl;

    int choice;
    while (true)
    {
        if (!ConsoleInput::readInt(choice))
        {
            return 0;
        }

        if (choice >= 1 && choice <= static_cast<int>(monsters.size()) + 1)
        {
            break;
        }

        ConsoleInput::printInvalidInput();
    }

    if (choice == static_cast<int>(monsters.size()) + 1)
    {
        ConsoleColor::printLine("\nLeaving the caves...", ConsoleColor::Color::Blue);
        engine.addAction("Left the caves without fighting");
        return 0;
    }

    Monster selectedMonster = monsters[choice - 1];
    engine.addAction("Chose to fight " + selectedMonster.getName() + " in the caves");

    if (!player.canFightMonster(selectedMonster))
    {
        ConsoleColor::printLine("You are not strong enough to fight this monster!", ConsoleColor::Color::DarkCyan);
        engine.addAction("Could not fight " + selectedMonster.getName() + " because XP was too low");
        return 0;
    }

    if (CombatSystem::fight(player, selectedMonster, true))
    {
        engine.addAction("Defeated " + selectedMonster.getName());
    }
    else if (player.getHealth() > 0)
    {
        engine.addAction("Ran away from " + selectedMonster.getName());
    }
    else
    {
        engine.addAction("Player was defeated");
    }

    if (player.getHealth() > 0)
    {
        ConsoleColor::printLine("\nLeaving the caves...", ConsoleColor::Color::Blue);
    }

    return 0;
}
