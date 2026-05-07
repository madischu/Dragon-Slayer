#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <string>
#include <utility>
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
    void resetDefaultMap();

    void displayCurrentLocation() const;
    void displayAvailablePaths() const;
    std::vector<std::string> getAvailablePaths() const;
    std::vector<std::pair<std::string, std::string>> getAllPaths() const;

    bool moveToLocation(int choice);
    bool moveToLocationByName(const std::string& locationName, std::string& message);
    std::string getCurrentLocation() const;
};

#endif
