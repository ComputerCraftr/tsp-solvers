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
