#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location
{
private:
    std::string name;
    std::string description;

public:
    Location(std::string n = "", std::string desc = "");

    std::string getName() const;
    std::string getDescription() const;
    void enter() const;
    void exit() const;
};

#endif
