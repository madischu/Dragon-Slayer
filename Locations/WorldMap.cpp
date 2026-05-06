#include "WorldMap.h"
#include "../Main Game/ConsoleColor.h"
#include <iostream>

WorldMap::WorldMap()
{
    currentLocationIndex = 0;
}

int WorldMap::findLocationIndex(const std::string& locationName) const
{
    for (int i = 0; i < locations.size(); i++)
    {
        if (locations[i].name == locationName)
        {
            return i;
        }
    }

    return -1;
}

void WorldMap::addLocation(const std::string& locationName)
{
    if (findLocationIndex(locationName) == -1)
    {
        locations.push_back(LocationNode(locationName));
    }
}

void WorldMap::addPath(const std::string& from, const std::string& to)
{
    int fromIndex = findLocationIndex(from);
    int toIndex = findLocationIndex(to);

    if (fromIndex == -1 || toIndex == -1)
    {
        return;
    }

    locations[fromIndex].connectedLocations.push_back(toIndex);
    locations[toIndex].connectedLocations.push_back(fromIndex);
}

void WorldMap::buildDefaultMap()
{
    addLocation("Town Square");
    addLocation("Store");
    addLocation("Ancient Ruins");
    addLocation("Dark Forest");
    addLocation("Caves");
    addLocation("Dragon's Lair");

    addPath("Town Square", "Store");
    addPath("Town Square", "Ancient Ruins");
    addPath("Town Square", "Dark Forest");
    addPath("Town Square", "Caves");
    addPath("Ancient Ruins", "Dark Forest");
    addPath("Ancient Ruins", "Dragon's Lair");
    addPath("Dark Forest", "Caves");
    addPath("Caves", "Dragon's Lair");

    currentLocationIndex = findLocationIndex("Town Square");
}

void WorldMap::displayCurrentLocation() const
{
    std::cout << "\nCurrent Location: "
              << locations[currentLocationIndex].name
              << std::endl;
}

void WorldMap::displayAvailablePaths() const
{
    ConsoleColor::printLine("\nAvailable paths:", ConsoleColor::Color::Magenta);

    for (int i = 0; i < locations[currentLocationIndex].connectedLocations.size(); i++)
    {
        int connectedIndex = locations[currentLocationIndex].connectedLocations[i];

        std::cout << i + 1 << ": "
                  << locations[connectedIndex].name
                  << std::endl;
    }
}

std::vector<std::string> WorldMap::getAvailablePaths() const
{
    std::vector<std::string> paths;

    for (int connectedIndex : locations[currentLocationIndex].connectedLocations)
    {
        paths.push_back(locations[connectedIndex].name);
    }

    return paths;
}

std::vector<std::pair<std::string, std::string>> WorldMap::getAllPaths() const
{
    std::vector<std::pair<std::string, std::string>> paths;

    for (int fromIndex = 0; fromIndex < static_cast<int>(locations.size()); ++fromIndex)
    {
        for (int toIndex : locations[fromIndex].connectedLocations)
        {
            if (fromIndex < toIndex)
            {
                paths.push_back({ locations[fromIndex].name, locations[toIndex].name });
            }
        }
    }

    return paths;
}

bool WorldMap::moveToLocation(int choice)
{
    int pathIndex = choice - 1;

    if (pathIndex < 0 || pathIndex >= locations[currentLocationIndex].connectedLocations.size())
    {
        return false;
    }

    currentLocationIndex = locations[currentLocationIndex].connectedLocations[pathIndex];

    return true;
}

bool WorldMap::moveToLocationByName(const std::string& locationName, std::string& message)
{
    for (int i = 0; i < static_cast<int>(locations[currentLocationIndex].connectedLocations.size()); ++i)
    {
        int connectedIndex = locations[currentLocationIndex].connectedLocations[i];

        if (locations[connectedIndex].name == locationName)
        {
            return moveToLocation(i + 1);
        }
    }

    if (locationName == getCurrentLocation())
    {
        message = "You are already at " + locationName + ".";
        return false;
    }

    message = "You cannot travel directly to " + locationName + " from here.";
    return false;
}

std::string WorldMap::getCurrentLocation() const
{
    return locations[currentLocationIndex].name;
}
