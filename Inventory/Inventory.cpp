#include "Inventory.h"
#include <iostream>
#include <sstream>

Inventory::Inventory()
{
    head = nullptr;
    itemCount = 0;
}

Inventory::~Inventory()
{
    clear();
}

Inventory::Inventory(const Inventory& other)
{
    head = nullptr;
    itemCount = 0;
    copyFrom(other);
}

Inventory& Inventory::operator=(const Inventory& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }

    return *this;
}

void Inventory::addItem(const Item& item)
{
    Node* newNode = new Node(item);

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        Node* current = head;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = newNode;
    }

    itemCount++;
}

bool Inventory::removeItem(const std::string& itemName)
{
    if (head == nullptr)
    {
        return false;
    }

    if (head->item.getName() == itemName)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
        itemCount--;
        return true;
    }

    Node* current = head;

    while (current->next != nullptr)
    {
        if (current->next->item.getName() == itemName)
        {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            itemCount--;
            return true;
        }

        current = current->next;
    }

    return false;
}

bool Inventory::containsItem(const std::string& itemName) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->item.getName() == itemName)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

Item Inventory::getItem(const std::string& itemName) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->item.getName() == itemName)
        {
            return current->item;
        }

        current = current->next;
    }

    return Item();
}

bool Inventory::hasItemType(ItemType type) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->item.getType() == type)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

std::string Inventory::getInventoryText() const
{
    std::ostringstream output;

    if (head == nullptr)
    {
        output << "Inventory is empty.";
        return output.str();
    }

    output << "\nInventory:" << std::endl;

    Node* current = head;
    int index = 1;

    while (current != nullptr)
    {
        output << index << ". "
               << current->item.getName()
               << " | Type: " << current->item.getTypeAsString()
               << " | Damage: " << current->item.getValue();

        if (current->item.getEffect() != "")
        {
            output << " | Effect: " << current->item.getEffect();
        }

        output << std::endl;

        current = current->next;
        index++;
    }

    return output.str();
}

void Inventory::displayInventory() const
{
    std::cout << getInventoryText() << std::endl;
}

void Inventory::displayItemsByType(ItemType type) const
{
    Node* current = head;
    bool found = false;
    int index = 1;

    while (current != nullptr)
    {
        if (current->item.getType() == type)
        {
            std::cout << index << ". "
                      << current->item.getName()
                      << " | Value: " << current->item.getValue();

            if (current->item.getEffect() != "")
            {
                std::cout << " | Effect: " << current->item.getEffect();
            }

            std::cout << std::endl;
            found = true;
        }

        current = current->next;
        index++;
    }

    if (!found)
    {
        std::cout << "No items of that type found." << std::endl;
    }
}

int Inventory::size() const
{
    return itemCount;
}

bool Inventory::isEmpty() const
{
    return head == nullptr;
}

void Inventory::clear()
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    itemCount = 0;
}

void Inventory::copyFrom(const Inventory& other)
{
    Node* current = other.head;

    while (current != nullptr)
    {
        addItem(current->item);
        current = current->next;
    }
}
