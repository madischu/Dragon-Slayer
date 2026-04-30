#include "Location.h"
#include <iostream>

Location::Location(std::string n, std::string desc)
    : name(n), description(desc)
{
}

std::string Location::getName() const
{
    return name;
}

std::string Location::getDescription() const
{
    return description;
}

void Location::enter() const
{
    std::cout << "\nEntering " << getName() << "." << std::endl;
}

void Location::exit() const
{
    std::cout << "Exiting " << getName() << "." << std::endl;
}
