#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include <string>

class Inventory
{
private:
    struct Node
    {
        Item item;
        Node* next;

        Node(const Item& newItem)
        {
            item = newItem;
            next = nullptr;
        }
    };

    Node* head;
    int itemCount;

public:
    Inventory();
    ~Inventory();

    Inventory(const Inventory& other);
    Inventory& operator=(const Inventory& other);

    void addItem(const Item& item);
    bool removeItem(const std::string& itemName);
    bool containsItem(const std::string& itemName) const;
    Item getItem(const std::string& itemName) const;

    bool hasItemType(ItemType type) const;
    void displayInventory() const;
    void displayItemsByType(ItemType type) const;

    int size() const;
    bool isEmpty() const;

private:
    void clear();
    void copyFrom(const Inventory& other);
};

#endif