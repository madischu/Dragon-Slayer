#include "ActionStack.h"
#include <iostream>

ActionStack::ActionStack()
{
    top = nullptr;
    count = 0;
}

ActionStack::~ActionStack()
{
    clear();
}

void ActionStack::push(const std::string& action)
{
    Node* newNode = new Node(action);

    newNode->next = top;
    top = newNode;

    count++;
}

std::string ActionStack::pop()
{
    if (isEmpty())
    {
        return "";
    }

    Node* temp = top;
    std::string removedAction = temp->action;

    top = top->next;

    delete temp;
    count--;

    return removedAction;
}

std::string ActionStack::peek() const
{
    if (isEmpty())
    {
        return "";
    }

    return top->action;
}

bool ActionStack::isEmpty() const
{
    return top == nullptr;
}

int ActionStack::size() const
{
    return count;
}

void ActionStack::displayRecentActions() const
{
    if (isEmpty())
    {
        std::cout << "No recent actions." << std::endl;
        return;
    }

    std::cout << "\nRecent Actions:" << std::endl;

    Node* current = top;
    int index = 1;

    while (current != nullptr)
    {
        std::cout << index << ". " << current->action << std::endl;

        current = current->next;
        index++;
    }
}

void ActionStack::clear()
{
    while (!isEmpty())
    {
        pop();
    }
}