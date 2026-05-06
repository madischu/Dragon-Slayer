#include "Player.h"
#include "../Main Game/ConsoleColor.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

Player::Player() : xp(0), health(100), gold(25), equippedWeapon("Stick", 5, "blunt")
{
    inventory.addItem(Item("Stick", ItemType::Weapon, 5, "Basic blunt weapon"));
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

Weapon Player::getCurrentWeapon() const
{
    return equippedWeapon;
}

std::string Player::getStatsText() const
{
    std::ostringstream output;
    output << "=========================" << std::endl;
    output << "Health: " << health << std::endl;
    output << "Gold: " << gold << std::endl;
    output << "XP: " << xp << std::endl;
    output << "Current Weapon: " << equippedWeapon.getName() << std::endl;
    output << "Inventory Items: " << inventory.size() << std::endl;
    output << "=========================";
    return output.str();
}

Inventory& Player::getInventory()
{
    return inventory;
}

const Inventory& Player::getInventory() const
{
    return inventory;
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

void Player::equipWeapon(const std::string& weaponName)
{
    if (!inventory.containsItem(weaponName))
    {
        std::cout << "You do not have that item." << std::endl;
        return;
    }

    Item item = inventory.getItem(weaponName);

    if (item.getType() != ItemType::Weapon)
    {
        std::cout << item.getName() << " is not a weapon." << std::endl;
        return;
    }

    std::string weaponType = "blunt";

    if (weaponName == "Dagger" || weaponName == "Sword")
    {
        weaponType = "sharp";
    }

    equippedWeapon = Weapon(item.getName(), item.getValue(), weaponType);

    std::cout << "You equipped the " << equippedWeapon.getName() << "." << std::endl;
}

void Player::usePotion(const std::string& potionName)
{
    if (!inventory.containsItem(potionName))
    {
        std::cout << "You do not have that potion." << std::endl;
        return;
    }

    Item item = inventory.getItem(potionName);

    if (item.getType() != ItemType::Potion)
    {
        std::cout << item.getName() << " is not a potion." << std::endl;
        return;
    }

    addHealth(item.getValue());
    inventory.removeItem(potionName);

    std::cout << "You used " << potionName << " and restored "
              << item.getValue() << " health." << std::endl;
}

void Player::addItemToInventory(const Item& item)
{
    inventory.addItem(item);
}

void Player::removeItemFromInventory(const std::string& itemName)
{
    if (inventory.removeItem(itemName))
    {
        std::cout << itemName << " was removed from your inventory." << std::endl;
    }
    else
    {
        std::cout << itemName << " was not found in your inventory." << std::endl;
    }
}

void Player::displayInventory() const
{
    inventory.displayInventory();
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
        ConsoleColor::printLine("\nYou dodge the attack from the " + monster.getName() + "!\n", ConsoleColor::Color::Blue);
        return true;
    }

    ConsoleColor::printLine("\nYou failed to dodge!", ConsoleColor::Color::Blue);
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

    ConsoleColor::printLine("You defeated the " + monster.getName() + "!\n", ConsoleColor::Color::Blue);

    if (monster.getName() == "Slime")
    {
        inventory.addItem(Item("Slime Gel", ItemType::Treasure, 5, "Can be sold later"));
        ConsoleColor::printLine("You found Slime Gel.\n", ConsoleColor::Color::DarkMagenta);
    }
    else if (monster.getName() == "Fanged Beast")
    {
        inventory.addItem(Item("Beast Fang", ItemType::Treasure, 15, "A sharp monster trophy"));
        ConsoleColor::printLine("You found a Beast Fang.\n", ConsoleColor::Color::DarkMagenta);
    }
    else if (monster.getName() == "Ghoul")
    {
        inventory.addItem(Item("Ghoul Bone", ItemType::Treasure, 25, "A rare monster drop"));
        ConsoleColor::printLine("You found a Ghoul Bone.\n", ConsoleColor::Color::DarkMagenta);
    }
    else if (monster.getName() == "Dragon")
    {
        inventory.addItem(Item("Dragon Scale", ItemType::Treasure, 100, "Proof that you defeated the dragon"));
        ConsoleColor::printLine("You found a Dragon Scale.\n", ConsoleColor::Color::DarkMagenta);
    }

    displayStats();

    ConsoleColor::printLine("\nYou earned " + std::to_string(goldEarned) + " gold and "
              + std::to_string(monster.getLevel()) + " XP.", ConsoleColor::Color::DarkMagenta);
}

void Player::displayStats() const
{
    ConsoleColor::printLine(getStatsText(), ConsoleColor::Color::DarkYellow);
}
