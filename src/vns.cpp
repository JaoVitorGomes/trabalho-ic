#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

#include "../header/random.hpp"
#include "../header/Grafo.hpp"

using namespace std;


bool adicionarNoComRestricao(Grafo grafo, vector<int> &trip, int idNo1, int idNo2,int idNo3, int novoNo, int dia) {
    // Verifica se os nós fornecidos estão presentes na viagem
    auto it1 = find(trip.begin(), trip.end(), idNo1);
    auto it2 = find(trip.begin(), trip.end(), idNo2);

    // Se ambos os nós estiverem presentes na viagem, adiciona o novo nó
        // Encontra a posição dos nós na viagem
        auto pos1 = distance(trip.begin(), it1);
        auto pos2 = distance(trip.begin(), it2);

    float valor_caminho = grafo.calculaCustoTempo(trip);
    float limite_caminho = grafo.tempoDia(dia);

    bool verify = true;
    float tempo_caminho = 0;

        if((limite_caminho - valor_caminho) > 0){
            tempo_caminho = ((limite_caminho - valor_caminho));
                if((tempo_caminho - grafo.tempo1no(idNo1,novoNo, idNo2)) >= 0){
                    trip.insert(trip.begin() + idNo1,novoNo);
                }

        }

}

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

vector<int> transformaSolucao(Grafo grafo, vector<vector<int>> trips) {
    vector<int> solucao;

    for (int i = 0; i < trips.size(); i++) {
        for (int j = 0; j < trips[i].size(); j++) {
            if (j < trips[i].size() - 1 && grafo.noHotel(trips[i][j]) && grafo.noHotel(trips[i][j + 1])) {
                cout << "invalido" << endl;  // Mensagem de aviso para uma situação inválida
            } else {
                if (find(solucao.begin(), solucao.end(), trips[i][j]) == solucao.end()) {
                    solucao.push_back(trips[i][j]);  // Adiciona o elemento ao vetor de solução se não estiver presente
                }
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
    int counter = 0;
    do{
    novas_trips = trips;
    cout << "tamanho trip" << trips.size() << endl;
    for (int i = 0; i < trips.size(); i++) {
        do {
            no = Random::get(2, ids - 1);
        } while (grafo.noHotel(no));
        cout << "tamanho trip "<< trips[i].size() << endl;
        if (trips[i].size() < 3) {
            cout << "é menor q 3" << endl;
            if(trips[i].size() == 0){
                trips.erase(trips.begin() + i);

            }
        } else {
            notrip = Random::get(1, trips[i].size() - 2);

  //          //bool valor = adicionarNoComRestricao(grafo, trips[i],)
  //          vector<int> used;
 //           for (auto &num : novas_trips[i]) {
 //               if (num != no && find(used.begin(), used.end(), num) == used.end()) {
 //                   used.push_back(num);
 //               } else {
 //                   while (find(used.begin(), used.end(), no) != used.end()) {
 //                       no = Random::get(2, ids - 1);
 //                   }
 //                   used.push_back(no);
 //               }
 //           }
            novas_trips[i][notrip] = no;
    }
    }
    counter++;
    nova_solucao = transformaSolucao(grafo, novas_trips);
    cout << grafo.validarSolucao(nova_solucao) << endl;
}while(!grafo.validarSolucao(nova_solucao) && counter < 10);
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
