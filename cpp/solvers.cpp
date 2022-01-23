// Copyright (c) 2022 John "ComputerCraftr" Studnicka

#include <solvers.h>

#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <vector>

// Sorted O(n*log(n)) TSP solver
double TSPSolverSimpleSorted(std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end());

    return TSPSolverSimple(citiesVec);
}

// Deterministic greedy O(n^2) TSP solver
double TSPSolverGreedySorted(std::vector<City>& citiesVec)
{
    // Set starting city to one near the middle
    std::sort(citiesVec.begin(), citiesVec.end());
    std::iter_swap(citiesVec.begin(), citiesVec.end() - 1 - citiesVec.size() / 2);

    std::unordered_map<uint32_t, bool> visited = {
        {0, true},
    };
    uint32_t lastVisited = 0;
    double totalDist = 0;

    std::vector<City> bestPath;
    bestPath.reserve(citiesVec.size());
    bestPath.push_back(citiesVec[0]);

    while (visited.size() < citiesVec.size()) {
        double minDist = std::numeric_limits<int32_t>::max();
        uint32_t lowestDistIndex = 0;

        for (int j = 0; j < citiesVec.size(); j++) {
            const double dist = citiesVec[lastVisited].DistTo(citiesVec[j]);

            // Use y coordinate as a tie breaker in case distances are equal
            if (visited.find(j) == visited.end() && (dist < minDist || (dist == minDist && citiesVec[j].GetY() < citiesVec[lowestDistIndex].GetY()))) {
                minDist = dist;
                lowestDistIndex = j;
            }
        }

        visited[lowestDistIndex] = true;
        lastVisited = lowestDistIndex;
        bestPath.push_back(citiesVec[lowestDistIndex]);
        totalDist += minDist;
    }

    // Add return trip
    totalDist += bestPath[bestPath.size() - 1].DistTo(bestPath[0]);
    citiesVec = bestPath;
    return totalDist;
}

// Brute force O(n!) TSP solver
double TSPSolverBruteForceSorted(std::vector<City>& citiesVec)
{
    std::sort(citiesVec.begin(), citiesVec.end());

    double minDist = std::numeric_limits<int32_t>::max();
    std::vector<City> bestPath = citiesVec;
    const City& firstCity = bestPath.front();

    do {
        const double dist = TSPSolverSimple(citiesVec);
        if (dist < minDist) {
            minDist = dist;
            bestPath = citiesVec;
        }
    } while (std::next_permutation(citiesVec.begin(), citiesVec.end()) && citiesVec.front() == firstCity); // If return trip is added, we can fix the first city because it will always have two connecting trips

    citiesVec = bestPath;
    return minDist;
}

double EvenFullGridSolverSorted(std::vector<City>& citiesVec, const City& entrancePoint, const City& exitPoint)
{
    int32_t xSmallest = std::numeric_limits<int32_t>::max();
    int32_t xLargest = 0;
    int32_t ySmallest = std::numeric_limits<int32_t>::max();
    int32_t yLargest = 0;

    int entranceIndex = -1;
    int exitIndex = -1;
    for (int i = 0; i < citiesVec.size(); i++) {
        xSmallest = std::min(xSmallest, citiesVec[i].GetX());
        xLargest = std::max(xLargest, citiesVec[i].GetX());

        ySmallest = std::min(ySmallest, citiesVec[i].GetY());
        yLargest = std::max(yLargest, citiesVec[i].GetY());

        if (entranceIndex == -1 && citiesVec[i] == entrancePoint) {
            entranceIndex = i;
        } else if (exitIndex == -1 && citiesVec[i] == exitPoint) {
            exitIndex = i;
        }
    }

    if (entranceIndex != -1 && exitIndex != -1 && entranceIndex != exitIndex &&
        (entrancePoint.GetX() == xSmallest || entrancePoint.GetX() == xLargest || // Ensure entrance and exit points are on the outside of the grid
         entrancePoint.GetY() == ySmallest /*|| entrancePoint.GetY() == yLargest*/) &&
        (exitPoint.GetX() == xSmallest || exitPoint.GetX() == xLargest ||
         exitPoint.GetY() == ySmallest /*|| exitPoint.GetY() == yLargest*/) &&
        entrancePoint.DistTo(exitPoint) == 1) {
        std::iter_swap(citiesVec.begin(), citiesVec.begin() + entranceIndex); // Set entrance point
        if (exitIndex == 0) {
            exitIndex = entranceIndex; // Update with new index if first element is swapped
        }
        std::iter_swap(citiesVec.end() - 1, citiesVec.begin() + exitIndex); // Set exit point
    } else {
        return std::numeric_limits<int32_t>::max();
    }

    const int32_t width = xLargest - xSmallest;
    const int32_t height = yLargest - ySmallest;
    int32_t xOffset = entrancePoint.GetX() - xSmallest; // Even go down, odd go up
    int32_t yOffset = entrancePoint.GetY() - ySmallest; // Zero goes right
    //int32_t totalDist = 1; // The entrance city is skipped because it has already been set at the beginning of the vector

    const int32_t lastCity = citiesVec.size() - 1;
    for (int i = 1; i < lastCity; i++) {
        if (yOffset == 0 && xOffset < width) { // Bottom of grid
            xOffset++;
        } else if ((xOffset == width || (xOffset & 1)) && yOffset < height) { // Right side, going up
            yOffset++;
        } else if ((yOffset == height && (xOffset & 1)) || (yOffset == 1 && !(xOffset & 1))) { // Top of grid or row above bottom
            xOffset--;
        } else if (!(xOffset & 1) && yOffset > 1) { // Going down
            yOffset--;
        }
        citiesVec[i] = City(xSmallest + xOffset, ySmallest + yOffset);
        //totalDist++;
    }

    const int32_t totalDist = lastCity; // Distance traveled in a full grid is equal to number of points minus one (the distance between entrance and exit points)

    return totalDist;
}

// Simple O(n) solver which can find an optimal path through a grid of cities
double FullGridSolverSorted(std::vector<City>& citiesVec, const City& entrancePoint, const City& exitPoint)
{
    if (citiesVec.size() & 1) { // Odd number of cities
        return 0;
    } else {
        const int32_t distFromExit = 1; // When entrance and exit are adjacent the return trip is 1 tile

        return EvenFullGridSolverSorted(citiesVec, entrancePoint, exitPoint) + distFromExit;
    }
}
