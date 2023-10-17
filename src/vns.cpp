#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

#include "../header/random.hpp"
#include "../header/Grafo.hpp"

using namespace std;

vector<vector<int>> transformaTrip(Grafo grafo, vector<int> solution){
    cout << "entrou no transforma" << endl;
    vector<vector<int>> trips;
    int dias_percorridos = 0;
    int tam_trip = grafo.tamanhoTrip();

    // Inicializando os vetores internos
    for (int i = 0; i < tam_trip; i++) {
        vector<int> trip;
        trips.push_back(trip);
    }

    trips[0].push_back(solution[0]);
    for(int i = 1; i < solution.size()-1; i++){
        if(grafo.noHotel(solution[i])){
            trips[dias_percorridos].push_back(solution[i]);
            dias_percorridos++;
        }
        trips[dias_percorridos].push_back(solution[i]);
    }
    trips[dias_percorridos].push_back(solution[solution.size()-1]); // Adiciona o último elemento

    cout << "saiu do transforma" << endl;
    return trips;
}

vector<int> transformaSolucao( vector<vector<int>> trips){
    vector<int> solucao;

    for(int i = 0; i  < trips.size() ; i++){
        for(int j = 0; j < trips[i].size(); j++){
            solucao.push_back(trips[i][j]);
        }
    }

    return solucao;
}

vector<int> gerarSolucao(Grafo grafo, vector<vector<int>> trips, int ids)
{

    bool verify = false;
    vector<vector<int>> novas_trips;
    vector<int> nova_solucao;
    int no;
    int notrip;
    // do{
        novas_trips = trips;
        for(int i = 0; i < trips.size(); i++){
            no = Random::get(2, ids - 1);
            notrip = Random::get(1, trips[i].size() - 2);
            novas_trips[i][notrip] = no; 
        }
        nova_solucao = transformaSolucao(novas_trips);
        
    // }
    // while(!grafo.validarSolucao(nova_solucao));

    return nova_solucao;
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
    vector<vector<int>> trips = transformaTrip(grafo, solution);
    // cout << "quantas trips->" << resultado.size();
    //  for(int i = 0; i < resultado.size(); i++){
    //      cout << "trip" << i << ": " << resultado[i].size() << endl;
    //  }
    while (interacao <= maxIterations)
    {
        k = 1;
        while (k <= 10)
        {
            
            vector<int> nova_solucao;
            nova_solucao = gerarSolucao(grafo, trips, ids);
            trips = transformaTrip(grafo,nova_solucao);
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
    //return make_pair(melhor_solucao,1);
}
