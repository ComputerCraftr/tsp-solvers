// Copyright (c) 2022 John "ComputerCraftr" Studnicka

#include <tsp.h>
#include <solvers.h>

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    // Initialize RNG
    const int64_t nTime = time(nullptr);
    srand(nTime);
    //srand(1642255937);
    printf("nTime = %lu\n", nTime);

    // Generate data
    std::vector<City> cities;
    //GenerateCitiesRandom(12, cities, 20);
    GenerateCitiesGrid(4, 3, cities);

    printf("unsorted dist      = %f\n", TSPSolverSimple(cities));
    printf("simple solver dist = %f\n", TSPSolverSimpleSorted(cities));
    printf("greedy solver dist = %f\n", TSPSolverGreedySorted(cities));
    const int64_t startTime = time(nullptr);
    printf("brute force dist   = %f\n", TSPSolverBruteForceSorted(cities));
    const int64_t endTime = time(nullptr);
    printf("brute force time = %lu\n", time(nullptr) - startTime);
    printf("grid solver dist   = %f\n", FullGridSolverSorted(cities, City(1, 1), City(1, 2)));
    PrintCitiesList(cities);
    PrintCitiesGrid(cities);
}


void GenerateCitiesRandom(const uint32_t numCities, std::vector<City>& citiesVec, const uint8_t maxCoord)
{
    citiesVec.clear();
    citiesVec.reserve(numCities);

    for (int i = 0; i < numCities; i++) {
        citiesVec.emplace_back(rand() % maxCoord + 1, rand() % maxCoord + 1); // random coordinates from 1 to 255
    }
}

void GenerateCitiesGrid(const uint8_t width, const uint8_t height, std::vector<City>& citiesVec)
{
    citiesVec.clear();
    citiesVec.reserve(width * height);

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            citiesVec.emplace_back(j, i);
        }
    }
}

void SetFirstClosestToOrigin(std::vector<City>& citiesVec)
{
    std::vector<City>::iterator lowestDistIterator = citiesVec.begin();
    double minDist = std::numeric_limits<int32_t>::max();

    for (std::vector<City>::iterator citiesIterator = citiesVec.begin(); citiesIterator != citiesVec.end(); citiesIterator++) {
        double dist = citiesIterator->DistToOrigin();

        // Use y coordinate as a tie breaker in case distances are equal
        if (dist < minDist || (dist == minDist && citiesIterator->GetY() < lowestDistIterator->GetY())) {
            minDist = dist;
            lowestDistIterator = citiesIterator;
        }
    }

    std::iter_swap(citiesVec.begin(), lowestDistIterator);
}

void SortClosestToPoint(const Point& point, std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end(), [&] (const City &a, const City &b) -> bool {
        const double aDist = a.DistTo(point);
        const double bDist = b.DistTo(point);

        // Use y coordinate as a tie breaker in case distances are equal
        return aDist < bDist || (aDist == bDist && a.GetY() < b.GetY());
    });
}

void SortSmallestArithmeticMeanCoordinates(std::vector<City>& citiesVec)
{
    // This is the same as sorting by arithmetic mean or the smallest "distance to" (-inf, -inf)
    std::sort(citiesVec.begin(), citiesVec.end(), [&] (const City &a, const City &b) -> bool {
        const int32_t aArithMeanDoubled = a.GetX() + a.GetY();
        const int32_t bArithMeanDoubled = b.GetX() + b.GetY();

        // Use y coordinate as a tie breaker in case averages are equal
        return aArithMeanDoubled < bArithMeanDoubled || (aArithMeanDoubled == bArithMeanDoubled && a.GetY() < b.GetY());
    });
}

void SortClosestToOrigin(std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end(), [&] (const City &a, const City &b) -> bool {
        const double aDist = a.DistToOrigin();
        const double bDist = b.DistToOrigin();

        // Use y coordinate as a tie breaker in case distances are equal
        return aDist < bDist || (aDist == bDist && a.GetY() < b.GetY());
    });
}

void SortSmallestGeometricMeanCoordinates(std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end(), [&] (const City &a, const City &b) -> bool {
        const int32_t aGeomMeanSquared = a.GetX() * a.GetY();
        const int32_t bGeomMeanSquared = b.GetX() * b.GetY();

        return aGeomMeanSquared < bGeomMeanSquared || (aGeomMeanSquared == bGeomMeanSquared && a < b);
    });
}

void SortLargestCoordinate(std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end(), [&] (const City &a, const City &b) -> bool {
        const int32_t aMaxCoord = std::max(a.GetX(), a.GetY());
        const int32_t bMaxCoord = std::max(b.GetX(), b.GetY());

        return aMaxCoord < bMaxCoord || (aMaxCoord == bMaxCoord && a < b);
    });
}

void PrintCitiesList(const std::vector<City>& citiesVec, bool withDist)
{
    for (int i = 0; i < citiesVec.size(); i++) {
        if (withDist && i > 0) {
            printf("dist = %f\n", citiesVec[i-1].DistTo(citiesVec[i]));
        }
        printf("%s\n", citiesVec[i].ToString().c_str());
    }
}

void PrintCitiesGrid(const std::vector<City>& citiesVec)
{
    std::vector<int32_t> xCoords;
    std::vector<int32_t> yCoords;
    xCoords.reserve(citiesVec.size());
    yCoords.reserve(citiesVec.size());
    int32_t xSmallest = std::numeric_limits<int32_t>::max();
    int32_t xLargest = 0;
    int32_t ySmallest = std::numeric_limits<int32_t>::max();
    int32_t yLargest = 0;

    for (int i = 0; i < citiesVec.size(); i++) {
        xCoords.push_back(citiesVec[i].GetX());
        yCoords.push_back(citiesVec[i].GetY());

        xSmallest = std::min(xSmallest, citiesVec[i].GetX());
        xLargest = std::max(xLargest, citiesVec[i].GetX());

        ySmallest = std::min(ySmallest, citiesVec[i].GetY());
        yLargest = std::max(yLargest, citiesVec[i].GetY());
    }

    for (int y = yLargest; y >= ySmallest; y--) {
        for (int x = xSmallest; x <= xLargest; x++) {
            bool empty = true;

            for (int i = 0; i < xCoords.size(); i++) {
                if (xCoords[i] == x && yCoords[i] == y) {
                    empty = false;
                }
            }

            if (empty) {
                printf("-");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
}
