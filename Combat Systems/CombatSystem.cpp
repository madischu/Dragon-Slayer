#include "CombatSystem.h"
#include <cstdlib>
#include <iostream>

bool CombatSystem::fight(Player& player, Monster& monster, bool canRunAway)
{
    std::cout << "\nYou are fighting a " << monster.getName() << "." << std::endl;
    std::cout << "Monster Health: " << monster.getHealth() << "\n" << std::endl;

    while (player.getHealth() > 0 && monster.getHealth() > 0)
    {
        std::cout << "1: Attack" << std::endl;
        std::cout << "2: Dodge" << std::endl;
        std::cout << "3: Run Away" << std::endl;

        int action;
        std::cin >> action;

        if (action == 1)
        {
            playerAttack(player, monster);
        }
        else if (action == 2)
        {
            bool dodged = player.dodge(monster);

            if (dodged)
            {
                continue;
            }

            player.takeDamage(monster.attack());
            std::cout << "Health: " << player.getHealth() << "\n" << std::endl;
        }
        else if (action == 3)
        {
            if (canRunAway)
            {
                std::cout << "\nYou run away from the battle!" << std::endl;
                std::cout << "\nLeaving the caves..." << std::endl;
                return false;
            }

            std::cout << "You cannot run away from the Dragon's Lair!" << std::endl;
        }
        else
        {
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    return monster.getHealth() <= 0;
}

void CombatSystem::playerAttack(Player& player, Monster& monster)
{
    if (!player.canFightMonster(monster))
    {
        return;
    }

    Weapon currentWeapon = player.getCurrentWeapon();

    std::cout << "\nThe " << monster.getName() << " attacks!" << std::endl;
    std::cout << "\nYou attack with your " << currentWeapon.getName() << "!\n";

    int bonus = player.getXP() / 10;
    int baseDamage = currentWeapon.getPower();
    int totalDamage = baseDamage + bonus;
    int effectiveness = player.getWeaponEffectiveness(currentWeapon, monster);

    double effectivenessPercentage = 0;

    if (currentWeapon.getPower() != 0)
    {
        effectivenessPercentage = (static_cast<double>(effectiveness) / currentWeapon.getPower()) * 100;
    }

    int finalDamage = 0;

    if (baseDamage != 0)
    {
        finalDamage = (totalDamage * effectiveness) / baseDamage;
    }

    std::cout << "\n*************************" << std::endl;
    std::cout << "Base Damage: " << baseDamage << std::endl;
    std::cout << "XP Bonus: " << bonus << std::endl;
    std::cout << "Effectiveness: " << effectivenessPercentage << "%" << std::endl;
    std::cout << "Final Damage: " << finalDamage << std::endl;
    std::cout << "*************************" << std::endl;

    if (rand() % 5 == 0 && currentWeapon.getName() != "Stick")
    {
        std::cout << "\nYour " << currentWeapon.getName() << " broke!" << std::endl;
        player.removeItemFromInventory(currentWeapon.getName());
        player.equipWeapon("Stick");
        std::cout << "You switched to your " << player.getCurrentWeapon().getName() << "!" << std::endl;
    }

    if (player.isMonsterHit())
    {
        if (effectiveness < baseDamage)
        {
            std::cout << "\nYour weapon is not very effective against this monster!" << std::endl;
        }

        std::cout << "\nYou dealt " << finalDamage << " damage!" << std::endl;
        monster.takeDamage(finalDamage, currentWeapon.getType());
        std::cout << "\n" << monster.getName() << " now has " << monster.getHealth() << " HP left." << std::endl;
    }
    else
    {
        std::cout << "\nYou missed!\n" << std::endl;
    }

    if (monster.getHealth() <= 0)
    {
        player.defeatMonster(monster);
        return;
    }

    int monsterDamage = monster.attack();
    player.takeDamage(monsterDamage);
    std::cout << "\nYou took " << monsterDamage << " damage! Current health: " << player.getHealth() << std::endl;
}
