#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Function to calculate the cost of a solution
int calculateCost(vector<vector<int>> &solution) {
    int cost = 0;
    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            cost += solution[i][j];
        }
    }
    return cost;
}

// Function to perform the VND algorithm
void VND(vector<vector<int>> &solution) {
    int cost = calculateCost(solution);
    while (true) {
        bool improved = false;
        for (int i = 0; i < solution.size(); i++) {
            for (int j = 0; j < solution[i].size(); j++) {
                for (int k = 0; k < solution[i][j]; k++) {
                    solution[i][j]--;
                    int newCost = calculateCost(solution);
                    if (newCost < cost) {
                        cost = newCost;
                        improved = true;
                        break;
                    } else {
                        solution[i][j]++;
                    }
                }
                if (improved) break;
            }
            if (improved) break;
        }
        if (!improved) break;
    }
}

// Function to perform the VNS algorithm
void VNS(vector<vector<int>> &solution) {
    srand(time(0));
    int cost = calculateCost(solution);
    int bestCost = cost;
    while (true) {
        vector<vector<int>> newSolution = solution;
        int randomIndex = rand() % newSolution.size();
        newSolution[randomIndex][rand() % newSolution[randomIndex].size()]++;
        VND(newSolution);
        int newCost = calculateCost(newSolution);
        if (newCost < cost) {
            cost = newCost;
            solution = newSolution;
            if (cost < bestCost) {
                bestCost = cost;
            }
        }
        if (cost == bestCost) break;
    }
}

int main() {
    vector<vector<int>> solution = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    VNS(solution);
    cout << "The minimum cost solution is: " << endl;
    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
    cout << "The cost of this solution is: " << calculateCost(solution) << endl;
    return 0;
}