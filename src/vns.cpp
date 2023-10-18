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
    int tam_trip = 0;

    for(auto& sol : solution){
        if(grafo.noHotel(sol)){
            tam_trip++;
        }
    }

    // Inicializando os vetores internos
    for (int i = 0; i < tam_trip; i++) {
        vector<int> trip;
        trips.push_back(trip);
    }
    cout << "depois da init" << endl;
    trips[0].push_back(solution[0]);
    cout << "depois do 0" << endl; 

    vector<int> new_solution;
    new_solution.push_back(0);

    for(auto& test : solution){
        if(test > 0){
            new_solution.push_back(test);
        }
    }

    solution = new_solution;

    for(auto& sol : solution){
        cout << sol << "->";
    }
    cout << "tamanho do tour->" << solution.size() << endl;
    for(int i = 1; i < solution.size()-1; i++){
        //cout << "i->" << i << endl;
        if(grafo.noHotel(solution[i])){
            trips[dias_percorridos].push_back(solution[i]);
            dias_percorridos++;
        }
        trips[dias_percorridos].push_back(solution[i]);
    }
    cout << "depois de modificar" << endl;
    trips[dias_percorridos].push_back(solution[solution.size()-1]); // Adiciona o último elemento

    cout << "saiu do transforma" << endl;
    return trips;
}

vector<int> transformaSolucao( Grafo grafo,vector<vector<int>> trips){
    vector<int> solucao;

    for(int i = 0; i  < trips.size() ; i++){
        for(int j = 0; j < trips[i].size(); j++){
            if((j != (trips[i].size()-1) &&grafo.noHotel(trips[i][j]) && grafo.noHotel(trips[i][j+1]))){
            cout << "invalido" << endl;
        }else{
            solucao.push_back(trips[i][j]);
        }
    }

}
    return solucao;
}

vector<int> twoOptLocalSearch(Grafo grafo, vector<int> solution) {
    //cout << "entrou no 2-opt" << endl;


      //  cout << "antes do calcula tempo" << endl;
        int best_cost = grafo.calculaCustoTempo(solution);
        //cout << "depois do calcula tempo" << endl;
        //cout << "valor do solution:" << solution.size() << endl;
        if(solution.size() == 0){
            return solution;
        }

        for (int i = 1; i < solution.size() - 1; i++) {
            for (int j = i + 1; j < solution.size(); j++) {
                //cout <<"valor de i e j" << i << " -> " << j << endl; 
                vector<int> new_solution = solution;
                reverse(new_solution.begin() + i, new_solution.begin() + j); // Inverte o trecho entre i e j
                int new_cost = grafo.calculaCustoTempo(new_solution);
                if (new_cost < best_cost) {
                    solution = new_solution;
                    best_cost = new_cost;

                }
            }
        }
        //cout << "saiu do 2-opt" << endl;
    
    return solution;
}

vector<int> gerarSolucao(Grafo grafo, vector<vector<int>> trips, int ids)
{

    bool verify = false;
    vector<vector<int>> novas_trips;
    vector<int> nova_solucao;
    int no;
    int notrip;
     //do{
        novas_trips = trips;
        //cout << "antes do for" << endl;
        for(int i = 0; i < trips.size(); i++){
            do{
                no = Random::get(2, ids - 1);
            }while(grafo.noHotel(no));
            
            //cout << "passou do 1" << endl;
            //cout << "trip size" << trips[i].size() << endl;
            if(trips[i].size() < 3){
                if(i == trips.size()){
                    //trips.clear();
                }else{
                    if(trips[i].size() != 0){
                        trips[i].clear();
                        for(auto& tri : trips[i+1]){
                            trips[i].push_back(tri);
                        }
                    }

                }
            }else{
                notrip = Random::get(1, trips[i].size() - 2);
                for(auto& tr : novas_trips[i]){
                    //cout << "valor:" << tr << endl;
                }
                novas_trips[i][notrip] = no; 
            }

        }
        //cout << "depois do for" << endl;
        nova_solucao = transformaSolucao(grafo,novas_trips);
        cout << grafo.validarSolucao(nova_solucao) << endl;  
     //}
     //while(!grafo.validarSolucao(nova_solucao));
     cout << "----------------->depoid do while <-------------------------------" << endl;
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
cout << "entrou na optimização" << endl;
        for(int i = 0; i < trips.size(); i++){
            trips[i] =  twoOptLocalSearch(grafo,trips[i]);
        }
cout << "saiu da optimização" << endl;
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
            cout << "antes gerar" << endl;
            nova_solucao = gerarSolucao(grafo, trips, ids);
            cout << "depois gerar" << endl;
            trips = transformaTrip(grafo,nova_solucao);
            for(int i = 0; i < trips.size(); i++){
                trips[i] =  twoOptLocalSearch(grafo, trips[i]);
            }
            cout << "depois do trip" << endl;
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
