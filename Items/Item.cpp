#include "Item.h"

Item::Item(std::string itemName, ItemType itemType, int itemValue, std::string itemEffect)
{
    name = itemName;
    type = itemType;
    value = itemValue;
    effect = itemEffect;
}

std::string Item::getName() const
{
    return name;
}

ItemType Item::getType() const
{
    return type;
}

int Item::getValue() const
{
    return value;
}

std::string Item::getEffect() const
{
    return effect;
}

std::string Item::getTypeAsString() const
{
    switch (type)
    {
    case ItemType::Weapon:
        return "Weapon";
    case ItemType::Potion:
        return "Potion";
    case ItemType::Armor:
        return "Armor";
    case ItemType::Treasure:
        return "Treasure";
    default:
        return "Unknown";
    }
}