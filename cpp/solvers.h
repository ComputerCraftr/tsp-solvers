// Copyright (c) 2022 John "ComputerCraftr" Studnicka

#ifndef SOLVERS_H
#define SOLVERS_H

#include <tsp.h>

#include <vector>

// Simple in order O(n) TSP solver
inline double TSPSolverSimple(const std::vector<City>& citiesVec)
{
    double totalDist = 0;

    for (int i = 1; i < citiesVec.size(); i++) {
        totalDist += citiesVec[i-1].DistTo(citiesVec[i]);
    }

    // Add return trip
    totalDist += citiesVec[citiesVec.size() - 1].DistTo(citiesVec[0]);
    return totalDist;
}

// Sorted O(n*log(n)) TSP solver
double TSPSolverSimpleSorted(std::vector<City>& citiesVec);

// Deterministic greedy O(n^2) TSP solver
double TSPSolverGreedySorted(std::vector<City>& citiesVec);

// Brute force O(n!) TSP solver
double TSPSolverBruteForceSorted(std::vector<City>& citiesVec);

double FullGridSolverSorted(std::vector<City>& citiesVec, const City& entrancePoint, const City& exitPoint);

#endif // SOLVERS_H
