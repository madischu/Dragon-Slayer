#include "Weapon.h"

Weapon::Weapon(std::string n, int p, std::string t)
    : name(n), power(p), type(t)
{
}

int Weapon::getPower() const
{
    return power;
}

std::string Weapon::getName() const
{
    return name;
}

std::string Weapon::getType() const
{
    return type;
}
