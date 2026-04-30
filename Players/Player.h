#ifndef PLAYER_H
#define PLAYER_H

#include "Weapon.h"
#include "Monster.h"
#include "Inventory.h"
#include <string>

class Player
{
private:
    int xp;
    int health;
    int gold;
    Weapon equippedWeapon;
    Inventory inventory;

public:
    Player();

    int getXP() const;
    int getHealth() const;
    int getGold() const;
    Weapon getCurrentWeapon() const;
    Inventory& getInventory();
    const Inventory& getInventory() const;

    void setHealth(int newHealth);
    void addHealth(int amount);
    void addGold(int amount);
    void subtractGold(int amount);
    void addXP(int amount);

    void equipWeapon(const std::string& weaponName);
    void usePotion(const std::string& potionName);
    void addItemToInventory(const Item& item);
    void removeItemFromInventory(const std::string& itemName);
    void displayInventory() const;

    void takeDamage(int damage);
    bool dodge(Monster& monster);
    bool isMonsterHit() const;
    bool canFightMonster(Monster& monster) const;
    int getWeaponEffectiveness(Weapon& weapon, Monster& monster) const;
    void defeatMonster(Monster& monster);
    void displayStats() const;
};

#endif
