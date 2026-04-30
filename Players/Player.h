#ifndef PLAYER_H
#define PLAYER_H

#include "Weapon.h"
#include "Monster.h"
#include <vector>

class Player
{
private:
    int xp;
    int health;
    int gold;
    int currentWeaponIndex;
    std::vector<Weapon> weapons;

public:
    Player();

    int getXP() const;
    int getHealth() const;
    int getGold() const;
    int getCurrentWeaponIndex() const;
    Weapon getCurrentWeapon() const;
    std::vector<Weapon>& getWeapons();

    void setHealth(int newHealth);
    void addHealth(int amount);
    void addGold(int amount);
    void subtractGold(int amount);
    void addXP(int amount);
    void setCurrentWeaponIndex(int index);

    void takeDamage(int damage);
    bool dodge(Monster& monster);
    bool isMonsterHit() const;
    bool canFightMonster(Monster& monster) const;
    int getWeaponEffectiveness(Weapon& weapon, Monster& monster) const;
    void defeatMonster(Monster& monster);
    void displayStats() const;
};

#endif
