#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

#include "../header/random.hpp"
#include "../header/Grafo.hpp"

using namespace std;

vector<int> gerarSolucao(vector<int> solution, vector<int> ids)
{
    int no1 = Random::get(2, ids.size() - 1);
    int no2 = Random::get(2, ids.size() - 1);

    int no3 = Random::get(1, solution.size() - 2);
    int no4 = Random::get(1, solution.size() - 2);

    solution[no3] = no1;
    solution[no4] = no2;

    return solution;
}

bool solucaoViavel(vector<int> solution, Grafo grafo)
{
}

// Function to perform a VND move
void performVNDMove(vector<int> ids, vector<int> &solution)
{
    int melhor_custo = calculaCusto(ids, solution);
    vector<int> melhor_solucao = solution;
    vector<int> nova_solucao;

    for (int i = 0; i < 20; i++)
    {

        nova_solucao = gerarSolucao(solution, ids);
        int custo_atual = calculateCost(nova_solucao);
        if (custo_atual < melhor_custo)
        {
            melhor_custo = custo_atual;
            melhor_solucao = solution;
        }
    }

    solution = melhor_solucao;
}

// Function to perform a VNS move
void performVNSMove(vector<int> &ids, vector<int> &solution)
{
    int melhor_custo = calculateCost(ids, solution);
    vector<int> melhor_solucao = solution;
    vector<int> nova_solucao;

    for (int i = 0; i < 20; i++)
    {

        nova_solucao = gerarSolucao(solution, ids);
        int custo_atual = calculateCost(nova_solucao);

        if (custo_atual < melhor_custo)
        {
            melhor_custo = custo_atual;
            melhor_solucao = solution;
        }
    }

    solution = melhor_solucao;
}

// Function to implement the VNS algorithm
vector<int> VNS(Grafo grafo, int ids, int maxIterations)
{
    std::pair<std::vector<int>, int> solucao = grafo.geraSolucao(0, 10, 1, 2, 0.6);
    vector<int> solution = solucao.first;
    int custo_atual = calculaCusto(solution);

    for (int i = 0; i < maxIterations; i++)
    {
        performVNDMove(ids, solution);
        performVNSMove(ids, solution);

        int novo_custo = cauculaCusto(solution);
        if (novo_custo < custo_atual)
        {
            custo_atual = novo_custo;
        }
        else
        {
            break;
        }
    }

    return solution;
}
