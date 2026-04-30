#include "Player.h"
#include <cstdlib>
#include <iostream>

Player::Player()
    : xp(0), health(100), gold(25), currentWeaponIndex(0)
{
    weapons.push_back(Weapon("Stick", 5, "blunt"));
}

int Player::getXP() const
{
    return xp;
}

int Player::getHealth() const
{
    return health;
}

int Player::getGold() const
{
    return gold;
}

int Player::getCurrentWeaponIndex() const
{
    return currentWeaponIndex;
}

Weapon Player::getCurrentWeapon() const
{
    return weapons[currentWeaponIndex];
}

std::vector<Weapon>& Player::getWeapons()
{
    return weapons;
}

void Player::setHealth(int newHealth)
{
    health = newHealth;

    if (health < 0)
    {
        health = 0;
    }

    if (health > 300)
    {
        health = 300;
    }
}

void Player::addHealth(int amount)
{
    setHealth(health + amount);
}

void Player::addGold(int amount)
{
    gold += amount;
}

void Player::subtractGold(int amount)
{
    gold -= amount;

    if (gold < 0)
    {
        gold = 0;
    }
}

void Player::addXP(int amount)
{
    xp += amount;
}

void Player::setCurrentWeaponIndex(int index)
{
    if (index >= 0 && index < static_cast<int>(weapons.size()))
    {
        currentWeaponIndex = index;
    }
}

void Player::takeDamage(int damage)
{
    health -= damage;

    if (health < 0)
    {
        health = 0;
    }
}

bool Player::dodge(Monster& monster)
{
    int dodgeChance = 20 + (xp / 10);

    if (dodgeChance > 50)
    {
        dodgeChance = 50;
    }

    if (rand() % 100 < dodgeChance)
    {
        std::cout << "\nYou dodge the attack from the " << monster.getName() << "!\n";
        return true;
    }

    std::cout << "\nYou failed to dodge!" << std::endl;
    return false;
}

bool Player::isMonsterHit() const
{
    if (health < 20)
    {
        return true;
    }

    return (rand() % 10) < 8;
}

bool Player::canFightMonster(Monster& monster) const
{
    if (monster.getName() == "Ghoul" && xp < 75)
    {
        std::cout << "\nYou need 75 XP to fight the Ghoul!" << std::endl;
        return false;
    }

    if (monster.getName() == "Dragon" && xp < 150)
    {
        std::cout << "\nYou need 150 XP to fight the Dragon!" << std::endl;
        return false;
    }

    return true;
}

int Player::getWeaponEffectiveness(Weapon& weapon, Monster& monster) const
{
    int effectiveness = weapon.getPower();

    if (monster.getName() == "Slime" || monster.getName() == "Ghoul")
    {
        if (weapon.getName() == "Dagger" || weapon.getName() == "Sword")
        {
            effectiveness /= 2;
        }
    }

    if (monster.getName() == "Fanged Beast")
    {
        if (weapon.getName() == "Stick" || weapon.getName() == "Claw Hammer")
        {
            effectiveness /= 2;
        }
    }

    if (monster.getName() == "Dragon" && weapon.getName() != "Sword")
    {
        effectiveness = 0;
    }

    return effectiveness;
}

void Player::defeatMonster(Monster& monster)
{
    int goldEarned = static_cast<int>(monster.getLevel() * 6.7);
    gold += goldEarned;
    xp += monster.getLevel();

    std::cout << "You defeated the " << monster.getName() << "!\n" << std::endl;
    displayStats();
    std::cout << "\nYou earned " << goldEarned << " gold and " << monster.getLevel() << " XP." << std::endl;
    std::cout << "\nLeaving the caves..." << std::endl;
}

void Player::displayStats() const
{
    std::cout << "=========================" << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "Gold: " << gold << std::endl;
    std::cout << "XP: " << xp << std::endl;
    std::cout << "Current Weapon: " << weapons[currentWeaponIndex].getName() << std::endl;
    std::cout << "=========================" << std::endl;
}
