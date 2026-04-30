#ifndef ITEM_H
#define ITEM_H

#include <string>

enum class ItemType
{
    Weapon,
    Potion,
    Armor,
    Treasure
};

class Item
{
private:
    std::string name;
    ItemType type;
    int value;
    std::string effect;

public:
    Item(std::string itemName = "",
         ItemType itemType = ItemType::Treasure,
         int itemValue = 0,
         std::string itemEffect = "");

    std::string getName() const;
    ItemType getType() const;
    int getValue() const;
    std::string getEffect() const;

    std::string getTypeAsString() const;
};

#endif