#include "Monster.h"
#include <cstdlib>
#include <iostream>

Monster::Monster(std::string n, int lvl, int hp, std::string rwt)
    : name(n), level(lvl), health(hp), requiredWeaponType(rwt)
{
}

std::string Monster::getName() const
{
    return name;
}

int Monster::getLevel() const
{
    return level;
}

int Monster::getHealth() const
{
    return health;
}

std::string Monster::getRequiredWeaponType() const
{
    return requiredWeaponType;
}

void Monster::takeDamage(int damage, std::string weaponType)
{
    health -= damage;

    if (health < 0)
    {
        health = 0;
    }
}

int Monster::attack()
{
    int baseDamage = level * 2;

    if (name == "Fanged Beast")
    {
        baseDamage += 5;
    }
    else if (name == "Ghoul")
    {
        baseDamage += 10;
    }
    else if (name == "Dragon")
    {
        baseDamage += 25;
    }

    int variation = rand() % (baseDamage / 5 + 1);
    bool increase = rand() % 2;

    if (increase)
    {
        baseDamage += variation;
    }
    else
    {
        baseDamage -= variation;
    }

    if (baseDamage < 0)
    {
        baseDamage = 0;
    }

    std::cout << name << " attacks for " << baseDamage << " damage!" << std::endl;
    return baseDamage;
}
