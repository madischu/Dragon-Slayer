#include "ActionStack.h"
#include <iostream>
#include <sstream>

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

std::vector<std::string> ActionStack::getRecentActions() const
{
    std::vector<std::string> actions;
    Node* current = top;

    while (current != nullptr)
    {
        actions.push_back(current->action);
        current = current->next;
    }

    return actions;
}

std::string ActionStack::getLogText() const
{
    if (isEmpty())
    {
        return "No recent actions.";
    }

    std::ostringstream output;
    output << "\nRecent Actions:\n";

    Node* current = top;

    while (current != nullptr)
    {
        output << "- " << current->action << "\n";
        current = current->next;
    }

    return output.str();
}

void ActionStack::displayRecentActions() const
{
    std::cout << getLogText() << std::endl;
}

void ActionStack::clear()
{
    while (!isEmpty())
    {
        pop();
    }
}
