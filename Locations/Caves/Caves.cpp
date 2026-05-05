#include "Caves.h"
#include "CombatSystem.h"
#include "EnemyQueue.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

Caves::Caves()
{
}

int Caves::enter(GameEngine& engine)
{
    Player& player = engine.getPlayer();

    std::cout << "\nYou enter a cave. You see some monsters.\n" << std::endl;
    engine.addAction("Entered the caves");

    if ((rand() % 100) < 25)
    {
        engine.addAction("Started a cave enemy wave");

        EnemyQueue enemies;
        enemies.buildCaveWaveForWeapon(player.getCurrentWeapon());

        while (!enemies.isEmpty() && player.getHealth() > 0)
        {
            Monster currentMonster = enemies.dequeue();
            std::cout << "\nNext enemy: " << currentMonster.getName() << std::endl;
            engine.addAction("Encountered " + currentMonster.getName());

            if (!player.canFightMonster(currentMonster))
            {
                std::cout << "You are not strong enough to fight this monster!" << std::endl;
                engine.addAction("Could not fight " + currentMonster.getName() + " because XP was too low");
                return 0;
            }

            if (!CombatSystem::fight(player, currentMonster, true, false))
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
            engine.addAction("Completed a cave enemy wave");
        }

        return 0;
    }

    EnemyQueue enemies;
    enemies.buildCaveWave();

    std::vector<Monster> monsters;
    while (!enemies.isEmpty())
    {
        monsters.push_back(enemies.dequeue());
    }

    std::cout << "Choose a monster to fight:" << std::endl;
    for (int i = 0; i < static_cast<int>(monsters.size()); ++i)
    {
        std::cout << i + 1 << ": " << monsters[i].getName() << std::endl;
    }
    std::cout << monsters.size() + 1 << ": Leave" << std::endl;

    int choice;
    while (true)
    {
        if (std::cin >> choice && choice >= 1 && choice <= static_cast<int>(monsters.size()) + 1)
        {
            break;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice! Choose again: ";
    }

    if (choice == static_cast<int>(monsters.size()) + 1)
    {
        std::cout << "\nLeaving the caves..." << std::endl;
        engine.addAction("Left the caves without fighting");
        return 0;
    }

    Monster selectedMonster = monsters[choice - 1];
    engine.addAction("Chose to fight " + selectedMonster.getName() + " in the caves");

    if (!player.canFightMonster(selectedMonster))
    {
        std::cout << "You are not strong enough to fight this monster!" << std::endl;
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

    return 0;
}
