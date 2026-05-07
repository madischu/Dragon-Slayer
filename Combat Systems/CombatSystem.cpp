#include "CombatSystem.h"
#include "../../Main Game/ConsoleColor.h"
#include "../../Main Game/ConsoleInput.h"
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
    // Enemy waves disable weapon breaking without changing the normal combat attack code path.
    bool weaponBreaksEnabled = true;
}

bool CombatSystem::fight(Player& player, Monster& monster, bool canRunAway, bool canWeaponBreak, bool showNextEnemy)
{
    weaponBreaksEnabled = canWeaponBreak;

    ConsoleColor::printLine("\nYou are fighting a " + monster.getName() + ".", ConsoleColor::Color::Blue);
    ConsoleColor::printLine("Monster Health: " + std::to_string(monster.getHealth()), ConsoleColor::Color::DarkRed);

    // Wave fights label the queued enemy after health so the combat header stays grouped.
    if (showNextEnemy)
    {
        ConsoleColor::printLine("\nNext enemy: " + monster.getName() + "\n", ConsoleColor::Color::Blue);
    }
    else
    {
        std::cout << std::endl;
    }

    while (player.getHealth() > 0 && monster.getHealth() > 0)
    {
        std::cout << "1: Attack" << std::endl;
        std::cout << "2: Dodge" << std::endl;
        std::cout << "3: Use Health Potion" << std::endl;
        std::cout << "4: Run Away" << std::endl;

        int action;
        if (!ConsoleInput::readInt(action))
        {
            weaponBreaksEnabled = true;
            return false;
        }

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
            player.usePotion("Health Potion");
        }
        else if (action == 4)
        {
            if (canRunAway)
            {
                ConsoleColor::printLine("\nYou run away from the battle!", ConsoleColor::Color::Blue);
                weaponBreaksEnabled = true;
                return false;
            }

            ConsoleColor::printLine("You cannot run away from this battle!", ConsoleColor::Color::DarkMagenta);
        }
        else
        {
            ConsoleInput::printInvalidInput();
        }
    }

    bool defeatedMonster = monster.getHealth() <= 0;
    weaponBreaksEnabled = true;
    return defeatedMonster;
}

void CombatSystem::playerAttack(Player& player, Monster& monster)
{
    if (!player.canFightMonster(monster))
    {
        return;
    }

    Weapon currentWeapon = player.getCurrentWeapon();

    ConsoleColor::printLine("\nThe " + monster.getName() + " attacks!", ConsoleColor::Color::DarkRed);
    ConsoleColor::printLine("\nYou attack with your " + currentWeapon.getName() + "!", ConsoleColor::Color::Blue);

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
        // Scale total damage by weapon effectiveness while avoiding division by zero.
        finalDamage = (totalDamage * effectiveness) / baseDamage;
    }

    ConsoleColor::printLine("\n*************************", ConsoleColor::Color::Cyan);
    std::cout << "Base Damage: " << baseDamage << std::endl;
    std::cout << "XP Bonus: " << bonus << std::endl;
    std::cout << "Effectiveness: " << effectivenessPercentage << "%" << std::endl;
    std::cout << "Final Damage: " << finalDamage << std::endl;
    ConsoleColor::printLine("*************************", ConsoleColor::Color::Cyan);

    if (weaponBreaksEnabled && rand() % 5 == 0 && currentWeapon.getName() != "Stick")
    {
        ConsoleColor::printLine("\nYour " + currentWeapon.getName() + " broke!", ConsoleColor::Color::Red);
        player.removeItemFromInventory(currentWeapon.getName());
        player.equipWeapon("Stick");
        ConsoleColor::printLine("You switched to your " + player.getCurrentWeapon().getName() + "!", ConsoleColor::Color::Blue);
    }

    if (player.isMonsterHit())
    {
        if (effectiveness < baseDamage)
        {
            ConsoleColor::printLine("\nYour weapon is not very effective against this monster!", ConsoleColor::Color::Red);
        }

        ConsoleColor::printLine("\nYou dealt " + std::to_string(finalDamage) + " damage!", ConsoleColor::Color::Blue);
        monster.takeDamage(finalDamage, currentWeapon.getType());
        ConsoleColor::printLine("\n" + monster.getName() + " now has " + std::to_string(monster.getHealth()) + " HP left.\n", ConsoleColor::Color::DarkRed);
    }
    else
    {
        ConsoleColor::printLine("\nYou missed!\n", ConsoleColor::Color::Red);
    }

    if (monster.getHealth() <= 0)
    {
        player.defeatMonster(monster);
        return;
    }

    int monsterDamage = monster.attack();
    player.takeDamage(monsterDamage);
    ConsoleColor::printLine("\nYou took " + std::to_string(monsterDamage) + " damage! Current health: " + std::to_string(player.getHealth()) + "\n", ConsoleColor::Color::Blue);
}
