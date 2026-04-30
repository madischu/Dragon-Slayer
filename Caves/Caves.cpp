#include "Caves.h"
#include "CombatSystem.h"
#include <iostream>

Caves::Caves()
    : Location("Caves", "You enter the caves. You see some monsters.")
{
}

int Caves::enter(Player& player)
{
    Location::enter();
    std::cout << "\nYou enter a cave. You see some monsters.\n" << std::endl;

    Monster monsters[] =
    {
        Monster("Slime", 2, 15, "blunt"),
        Monster("Fanged Beast", 8, 60, "sharp"),
        Monster("Ghoul", 20, 150, "blunt")
    };

    std::cout << "Choose a monster to fight:" << std::endl;
    std::cout << "1: Slime" << std::endl;
    std::cout << "2: Fanged Beast" << std::endl;
    std::cout << "3: Ghoul" << std::endl;
    std::cout << "4: Leave" << std::endl;

    int choice;

    while (true)
    {
        std::cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            break;
        }

        std::cout << "Invalid choice! Choose again: ";
    }

    if (choice == 4)
    {
        std::cout << "\nLeaving the caves..." << std::endl;
        return 0;
    }

    Monster& selectedMonster = monsters[choice - 1];

    if (!player.canFightMonster(selectedMonster))
    {
        std::cout << "You are not strong enough to fight this monster!" << std::endl;
        return 0;
    }

    CombatSystem::fight(player, selectedMonster, true);
    return 0;
}
