#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "../header/Grafo.hpp"

using namespace std;

// Function to calculate the cost of a solution
int calculateCost(vector<vector<int>>& tasks, vector<int>& solution) {
    int cost = 0;
    for (int i = 0; i < tasks.size(); i++) {
        cost += tasks[i][solution[i]];
    }
    return cost;
}

// Function to generate a random solution
vector<int> generateRandomSolution(vector<vector<int>>& tasks) {
    vector<int> solution(tasks.size(), 0);
    for (int i = 0; i < tasks.size(); i++) {
        solution[i] = rand() % tasks[i].size();
    }
    return solution;
}

// Function to perform a VND move
void performVNDMove(vector<vector<int>>& tasks, vector<int>& solution) {
    int bestCost = calculateCost(tasks, solution);
    vector<int> bestSolution = solution;

    for (int i = 0; i < tasks.size(); i++) {
        for (int j = 0; j < tasks[i].size(); j++) {
            if (j != solution[i]) {
                solution[i] = j;
                int currentCost = calculateCost(tasks, solution);
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    bestSolution = solution;
                }
            }
        }
    }

    solution = bestSolution;
}

// Function to perform a VNS move
void performVNSMove(vector<vector<int>>& tasks, vector<int>& solution) {
    int bestCost = calculateCost(tasks, solution);
    vector<int> bestSolution = solution;

    for (int i = 0; i < tasks.size(); i++) {
        for (int j = 0; j < tasks[i].size(); j++) {
            if (j != solution[i]) {
                solution[i] = j;
                int currentCost = calculateCost(tasks, solution);
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    bestSolution = solution;
                } else {
                    solution[i] = j;
                }
            }
        }
    }

    solution = bestSolution;
}

// Function to implement the VNS algorithm
vector<int> VNS(vector<vector<int>>& tasks, int maxIterations) {
    vector<int> solution = generateRandomSolution(tasks);
    int currentCost = calculateCost(tasks, solution);

    for (int i = 0; i < maxIterations; i++) {
        performVNDMove(tasks, solution);
        performVNSMove(tasks, solution);

        int newCost = calculateCost(tasks, solution);
        if (newCost < currentCost) {
            currentCost = newCost;
        } else {
            break;
        }
    }

    return solution;
}

int main() {
    srand(time(0));

    // Define the tasks and their costs
    vector<vector<int>> tasks = {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}
    };

    // Run the VNS algorithm
    vector<int> solution = VNS(tasks, 1000);

    // Print the solution and its cost
    cout << "Solution: ";
    for (int i : solution) {
        cout << i << " ";
    }
    cout << "\nCost: " << calculateCost(tasks, solution) << endl;

    return 0;
}