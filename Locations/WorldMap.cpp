#include "WorldMap.h"
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
    addLocation("Caves");
    addLocation("Dark Forest");
    addLocation("Ancient Ruins");
    addLocation("Dragon's Lair");

    addPath("Town Square", "Store");
    addPath("Town Square", "Caves");
    addPath("Caves", "Dark Forest");
    addPath("Dark Forest", "Ancient Ruins");
    addPath("Ancient Ruins", "Dragon's Lair");

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
    std::cout << "\nAvailable paths:" << std::endl;

    for (int i = 0; i < locations[currentLocationIndex].connectedLocations.size(); i++)
    {
        int connectedIndex = locations[currentLocationIndex].connectedLocations[i];

        std::cout << i + 1 << ": "
                  << locations[connectedIndex].name
                  << std::endl;
    }
}

bool WorldMap::moveToLocation(int choice)
{
    int pathIndex = choice - 1;

    if (pathIndex < 0 || pathIndex >= locations[currentLocationIndex].connectedLocations.size())
    {
        std::cout << "Invalid path choice." << std::endl;
        return false;
    }

    currentLocationIndex = locations[currentLocationIndex].connectedLocations[pathIndex];

    std::cout << "\nYou travel to "
              << locations[currentLocationIndex].name
              << "."
              << std::endl;

    return true;
}

std::string WorldMap::getCurrentLocation() const
{
    return locations[currentLocationIndex].name;
}