#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon
{
private:
    std::string name;
    int power;
    std::string type;

public:
    Weapon(std::string n = "Stick", int p = 5, std::string t = "blunt");

    int getPower() const;
    std::string getName() const;
    std::string getType() const;
};

#endif
