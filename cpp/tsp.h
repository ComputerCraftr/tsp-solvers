// Copyright (c) 2022 John "ComputerCraftr" Studnicka

#ifndef TSP_H
#define TSP_H

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

struct Point
{
    // Point coordinates
    int32_t xCoord;
    int32_t yCoord;

    Point()
        : xCoord(0), yCoord(0) {}

    Point(int32_t x, int32_t y)
        : xCoord(x), yCoord(y) {}
};

struct City
{
private:
    // City coordinates (always positive)
    Point coords;

public:
    City()
        : coords(0, 0) {}

    City(uint8_t x, uint8_t y)
        : coords(x, y) {}

    bool operator==(const City& otherCity) const
    {
        return coords.xCoord == otherCity.coords.xCoord && coords.yCoord == otherCity.coords.yCoord;
    }

    bool operator!=(const City& otherCity) const
    {
        return coords.xCoord != otherCity.coords.xCoord || coords.yCoord != otherCity.coords.yCoord;
    }

    bool operator<(const City& otherCity) const
    {
        // Sort by y and then by x
        return coords.yCoord < otherCity.coords.yCoord || (coords.yCoord == otherCity.coords.yCoord && coords.xCoord < otherCity.coords.xCoord);
    }

    bool operator<=(const City& otherCity) const
    {
        // Sort by y and then by x
        const bool yEqual = coords.yCoord == otherCity.coords.yCoord;
        return (coords.xCoord == otherCity.coords.xCoord && yEqual) ||
            coords.yCoord < otherCity.coords.yCoord || (yEqual && coords.xCoord < otherCity.coords.xCoord);
    }

    double DistTo(const Point& point) const
    {
        const int64_t xDiff = point.xCoord - coords.xCoord;
        const int64_t yDiff = point.yCoord - coords.yCoord;

        return std::sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    double DistTo(const City& otherCity) const
    {
        return DistTo(otherCity.coords);
    }

    double DistToOrigin() const
    {
        return std::sqrt(coords.xCoord * coords.xCoord + coords.yCoord * coords.yCoord);
    }

    double DistToDummy() const
    {
        const int32_t xDiff = 255 - coords.xCoord;
        const int32_t yDiff = coords.yCoord;

        return std::sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    int32_t GetX() const
    {
        return coords.xCoord;
    }

    int32_t GetY() const
    {
        return coords.yCoord;
    }

    std::string ToString() const
    {
        return "City (" + std::to_string(coords.xCoord) + ", " + std::to_string(coords.yCoord) + ")";
    }
};

void GenerateCitiesRandom(const uint32_t numCities, std::vector<City>& citiesVec, const uint8_t maxCoord = 255);
void GenerateCitiesGrid(const uint8_t width, const uint8_t height, std::vector<City>& citiesVec);

void SetFirstClosestToOrigin(std::vector<City>& citiesVec);

void SortClosestToPoint(const Point& point, std::vector<City>& citiesVec);
void SortSmallestArithmeticMeanCoordinates(std::vector<City>& citiesVec);
void SortClosestToOrigin(std::vector<City>& citiesVec);
void SortSmallestGeometricMeanCoordinates(std::vector<City>& citiesVec);
void SortLargestCoordinate(std::vector<City>& citiesVec);

void PrintCitiesList(const std::vector<City>& citiesVec, bool withDist = false);
void PrintCitiesGrid(const std::vector<City>& citiesVec);

#endif // TSP_H
