#ifndef ACTIONSTACK_H
#define ACTIONSTACK_H

#include <string>
#include <vector>

class ActionStack
{
private:
    struct Node
    {
        std::string action;
        Node* next;

        Node(std::string newAction)
        {
            action = newAction;
            next = nullptr;
        }
    };

    Node* top;
    int count;

public:
    ActionStack();
    ~ActionStack();

    void push(const std::string& action);
    std::string pop();
    std::string peek() const;

    bool isEmpty() const;
    int size() const;
    std::vector<std::string> getRecentActions() const;
    std::string getLogText() const;
    void displayRecentActions() const;
    void clear();
};

#endif
