#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <string>
#include <vector>

class WorldMap
{
private:
    struct LocationNode
    {
        std::string name;
        std::vector<int> connectedLocations;

        LocationNode(std::string locationName)
        {
            name = locationName;
        }
    };

    std::vector<LocationNode> locations;
    int currentLocationIndex;

    int findLocationIndex(const std::string& locationName) const;

public:
    WorldMap();

    void addLocation(const std::string& locationName);
    void addPath(const std::string& from, const std::string& to);
    void buildDefaultMap();

    void displayCurrentLocation() const;
    void displayAvailablePaths() const;

    bool moveToLocation(int choice);
    std::string getCurrentLocation() const;
};

#endif