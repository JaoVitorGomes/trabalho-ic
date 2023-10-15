#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Function to calculate the cost of a solution
int calculateCost(vector<vector<int>>& tasks, vector<int>& solution) {
    int cost = 0;
    for (int i = 0; i < tasks.size(); i++) {
        cost += tasks[i][solution[i]];
    }
    return cost;
}


// Function to perform a VND move
void VND(vector<vector<int>>& tasks, vector<int>& solution) {
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
