#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

#include "../header/random.hpp"
#include "../header/Grafo.hpp"

using namespace std;

vector<int> gerarSolucao(Grafo grafo, vector<int> solution, int ids)
{
    bool verify = false;
    vector<int> nova_solucao;

    while(verify == false){
        nova_solucao = solution;
        int no1 = Random::get(2, ids - 1);
        int no2 = Random::get(2, ids - 1);

        int no3 = Random::get(1, solution.size() - 2);
        int no4 = Random::get(1, solution.size() - 2);

        nova_solucao[no3] = no1;
        nova_solucao[no4] = no2;

        if(grafo.validarSolucao(nova_solucao)){
            verify = true;
        }
    }
    cout << nova_solucao.size() << endl;
    return nova_solucao;
}


// Function to perform a VND move
void performVNDMove(Grafo grafo, int ids, vector<int> &solution)
{
    int melhor_custo = grafo.calculaCusto(solution);
    vector<int> melhor_solucao = solution;
    vector<int> nova_solucao;

    for (int i = 0; i < 20; i++)
    {

        nova_solucao = gerarSolucao(grafo, solution, ids);
        int custo_atual = grafo.calculaCusto(solution);
        if (custo_atual < melhor_custo)
        {
            melhor_custo = custo_atual;
            melhor_solucao = nova_solucao;
        }
    }

    solution = melhor_solucao;
}

// Function to perform a VNS move
void performVNSMove(Grafo grafo,int ids, vector<int> &solution)
{
    int melhor_custo = grafo.calculaCusto(solution);
    vector<int> melhor_solucao = solution;
    vector<int> nova_solucao;

    for (int i = 0; i < 20; i++)
    {

        nova_solucao = gerarSolucao(grafo, solution, ids);
        int custo_atual = grafo.calculaCusto(solution);

        if (custo_atual < melhor_custo)
        {
            melhor_custo = custo_atual;
            melhor_solucao = nova_solucao;
        }
    }

    solution = melhor_solucao;
}

// Function to implement the VNS algorithm
vector<int> VNS(Grafo grafo, int ids, int maxIterations)
{
    auto solucao = grafo.geraSolucao(0.10);
    vector<int> solution = solucao.first;
    int custo_atual = grafo.calculaCusto(solution);

    for (int i = 0; i < maxIterations; i++)
    {
        //performVNDMove(grafo,ids, solution);
        performVNSMove(grafo,ids, solution);

        int novo_custo = grafo.calculaCusto(solution);
        if (novo_custo < custo_atual)
        {
            custo_atual = novo_custo;
        }
        else
        {
            break;
        }
    }
    auto custo = grafo.calculaCusto(solucao.first);
    std::cout<<"\nCusto solucao: "<<custo;
    return solution;
}
