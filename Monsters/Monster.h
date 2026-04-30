#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Monster
{
private:
    std::string name;
    int level;
    int health;
    std::string requiredWeaponType;

public:
    Monster(std::string n, int lvl, int hp, std::string rwt);

    std::string getName() const;
    int getLevel() const;
    int getHealth() const;
    std::string getRequiredWeaponType() const;

    void takeDamage(int damage, std::string weaponType);
    int attack();
};

#endif
