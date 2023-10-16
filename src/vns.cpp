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

    while (verify == false)
    {
        nova_solucao = solution;
        int no1 = Random::get(2, ids - 1);
        int no2 = Random::get(2, ids - 1);

        int no3 = Random::get(1, solution.size() - 2);
        int no4 = Random::get(1, solution.size() - 2);

        nova_solucao[no3] = no1;
        nova_solucao[no4] = no2;

        if (grafo.validarSolucao(nova_solucao))
        {
            verify = true;
        }
    }
    return nova_solucao;
}

// Function to perform a VND move
// void performVNDMove(Grafo grafo, int ids, vector<int> &solution)
// {
//     int melhor_custo = grafo.calculaCusto(solution);
//     vector<int> melhor_solucao = solution;
//     vector<int> nova_solucao;

//     for (int i = 0; i < 20; i++)
//     {

//         nova_solucao = gerarSolucao(grafo, solution, ids);
//         int custo_atual = grafo.calculaCusto(solution);
//         if (custo_atual < melhor_custo)
//         {
//             melhor_custo = custo_atual;
//             melhor_solucao = nova_solucao;
//         }
//     }

//     solution = melhor_solucao;
// }

// Function to perform a VNS move
void performVNSMove(Grafo grafo, int ids, vector<int> &solution)
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
pair<vector<int>, int> VNS(Grafo grafo, int ids, int maxIterations, vector<int> solution)
{

    int custo_atual = grafo.calculaCusto(solution);
    int qNo = grafo.quantidadeNos();
    int k = 0;
    int r = ((qNo * (qNo - 1)) / 2);
    int interacao = 0;
    vector<int> melhor_solucao = solution;
    int melhor_custo = grafo.calculaCusto(solution);

    while (interacao <= maxIterations)
    {
        k = 1;
        while (k <= 10)
        {
            
            vector<int> nova_solucao;
            nova_solucao = gerarSolucao(grafo, melhor_solucao, ids);
            int custo_atual = grafo.calculaCusto(nova_solucao);

            if (custo_atual > melhor_custo)
            {
                melhor_custo = custo_atual;
                melhor_solucao = nova_solucao;
                k = 1;
            }else{
                k++;
            }
            cout << "->" << k << endl;
        }
        interacao++;
    }
    auto custo = grafo.calculaCusto(melhor_solucao);
    std::cout << "\nCusto solucao: " << custo << endl;

    for(auto& val : melhor_solucao){
        cout << "->" << val;
    }
    return make_pair(melhor_solucao, custo);
}
