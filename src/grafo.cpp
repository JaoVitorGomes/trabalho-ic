#include "../header/Grafo.hpp"
#include "../header/random.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <execution>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <queue>
#include <random>
#include <stack>
#include <utility>
#include <vector>
#include <cstddef>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Grafo::Grafo(std::string filename){

    std::ifstream file(filename);
    std::string line, linha1,linha2,linha3;

    int num_vertices, num_hoteis_extras, numero_dias;
    std::vector<std::pair<int, int>> coordenadas_nos;
    std::vector<int> pesos;

    // Pega: número de vértice + 2, número de hoteis extra, número de trips = número de dias
    getline(file, line);
    linha1 = line;
    std::istringstream conversor(line);
    conversor >> num_vertices >> num_hoteis_extras >> numero_dias;
    
    // Tempo total
    getline(file, line);
    linha2 = line;

    // Tamanho para as trips = tempos limites de cada trip/dia 
    getline(file, line);
    linha3 = line;

    // Pegando o tempo limite de cada dia
    std::istringstream conversorLinha3(linha3);
    std::string token;
    
    while (std::getline(conversorLinha3, token, '\t'))
        this->tempos_limites_dias.push_back(std::stof(token));

    // Pega linha em branco
    getline(file,line);

    // Adicionando os nós
    while (getline(file, line)) {
      std::istringstream conversor(line);
      int x, y , peso;
      conversor >> x >> y >> peso;
      coordenadas_nos.push_back(std::make_pair(x, y));
      pesos.push_back(peso);

    }

    int contador = 0;

    for (auto no : coordenadas_nos) {

      Vertice novono;
      novono.id = contador;
      novono.coord = no;
      novono.peso = pesos[contador];

      if (novono.peso == 0){
        novono.is_hotel = true;
        this->ids_hoteis.push_back(novono.id);
      }

      this->adicionarNo(contador,novono);
      contador++;

    }

    this->ordem = num_vertices;
    this->comeco_fim = std::make_pair(0, 1);
    this->nos.reserve(this->ordem);
    this->tam_trip = numero_dias;

    // CADASTRANDO ARESTAS
    for ( auto noExterno : this->nos){
        for ( auto noInterno : this->nos){
          float pesoAresta = std::sqrt( std::pow((noExterno.second.coord.first - noInterno.second.coord.first), 2) + std::pow((noExterno.second.coord.second - noInterno.second.coord.second), 2) ); 
          this->adicionarAresta(noExterno.first, noInterno.first, pesoAresta);
        }
            
    }

    // Preenche a distância mais próxima até um hotel
    for(auto& elemento : this->nos)
      this->acha_hotel_mais_perto(elemento.second);
    
    // Preenche a distância mais próxima até o destino final
    for(auto& elemento : this->nos)
      this->acha_caminho_mais_proximo_final_hotel(elemento.second);

    // TESTES
    /*
      // Printando hotel
      for (auto& elemento : this->nos){

        std::cout << "\nNo: "<<elemento.second.id<< " Custo para o hotel mais proximo: "<<elemento.second.cost_nearest_hotel;
        std::cout<<"\nCaminho para o hotel mais proximo:\n";

        for(auto& elemento_caminho : elemento.second.path_nearest_hotel)
          std::cout<<elemento_caminho << "->";

      }
      for(auto& elemento : this->ids_hoteis)
        std::cout<<"\nId hotel: "<<elemento;

      //std::cout<<"\nNumero de nos: "<< this->ordem;
      
      //this->printa_arestas();
    */
}

void Grafo::adicionarNo(int id, Vertice vertice)
{
  nos.emplace(id, vertice);
}

void Grafo::acha_hotel_mais_perto(Vertice& no){

  std::vector<std::pair<int, std::vector<int>>> situacao_hoteis;
  for(auto hotel : this->ids_hoteis){
    auto [custo, caminho_hotel] = this->dijkstra(no.id, hotel);
    situacao_hoteis.push_back(std::make_pair(custo, caminho_hotel));
  }

  // Ordeno situacao_hoteis com base no custo em ordem crescente
  std::sort(situacao_hoteis.begin(), situacao_hoteis.end(),
            [](const std::pair<int, std::vector<int>>& a, const std::pair<int, std::vector<int>>& b) {
                return a.first < b.first;
            });

  no.cost_nearest_hotel = situacao_hoteis[0].first;
  no.path_nearest_hotel = situacao_hoteis[0].second;


}

void Grafo::acha_caminho_mais_proximo_final_hotel(Vertice& no){
  
  auto [custo, caminho] = this->dijkstra(no.id, this->comeco_fim.second);

  no.cost_last_destiny = custo;
  no.path_to_last_destiny = caminho;

}
void Grafo::printa_arestas() {
    for (const auto& pair : nos) {
        const Vertice& vertice = pair.second;
        std::cout << "\nArestas do Vértice " << vertice.id << ":\n";
        
        for (const Aresta& aresta : vertice.arestas) {
            std::cout << "  Destino: " << aresta.id << ", Peso: " << aresta.peso << "\n";
        }
    }
}


void Grafo::adicionarAresta(int origem, int destino, float peso)
{
  auto noOrigem = nos.find(origem);
  auto noDestino = nos.find(destino);
  if (noOrigem == nos.end() || noDestino == nos.end()) {
    return;
  }
  Aresta aresta{ destino, peso};
  noOrigem->second.arestas.push_front(aresta);
  
}

bool Grafo::removerNo(int id)
{
    return true;
}

void Grafo::removerAresta(int origem, int destino)
{
  auto noOrigem = nos.find(origem);
  auto noDestino = nos.find(destino);
  if (noOrigem == nos.end() || noDestino == nos.end()) {
    return;
  }

  auto& arestas = noOrigem->second.arestas;
  auto numRemovidos = std::count_if(arestas.begin(), arestas.end(), [destino](const Aresta& aresta) { return aresta.id == destino; });

  if (numRemovidos == 0)
    return;
  
  noDestino->second.arestas.remove_if([origem](const Aresta& aresta) { return aresta.id == origem; });
  
}

Vertice& Grafo::no(int id){
  return nos.at(id);
}

std::pair<int, std::vector<int>> Grafo::dijkstra(int origem, int destino) {
    int ordem = nos.size();
    auto noOrigem = nos.find(origem);
    auto noDestino = nos.find(destino);
    if (ordem <= 1)
        return std::make_pair(0, std::vector<int>());

    //Vertice& noOrigem = noOrigem->second;
    //Vertice& noDestino = noDestino->second;

    std::unordered_map<int, int> distancias;
    std::unordered_map<int, int> antecessores;

    for (const auto& par : nos) {
        distancias[par.first] = std::numeric_limits<int>::max();
        antecessores[par.first] = -1; // Inicialmente não possui antecessor.
    }

    distancias[origem] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> fila;

    fila.push(std::make_pair(0, origem));

    while (!fila.empty()) {
        int distanciaAtual = fila.top().first;
        int idAtual = fila.top().second;
        fila.pop();

        if (idAtual == destino) {
            break;
        }

        Vertice& noAtual = nos[idAtual];

        for (const auto& aresta : noAtual.arestas) {
            int idAdjacente = aresta.id;
            int pesoAteAdjacente = aresta.peso;
            Vertice& noAdjacente = nos[idAdjacente];

            int novaDistancia = distanciaAtual + pesoAteAdjacente;

            if (novaDistancia < distancias[idAdjacente]) {
                distancias[idAdjacente] = novaDistancia;
                antecessores[idAdjacente] = idAtual;
                fila.push(std::make_pair(novaDistancia, idAdjacente));
            }
        }
    }

    // Construir o vetor do caminho mínimo a partir dos antecessores
    std::vector<int> caminho;
    int noAtual = destino;
    while (noAtual != -1) {
        caminho.push_back(noAtual);
        noAtual = antecessores[noAtual];
    }

    // Inverter o vetor, já que a construção começa pelo destino
    std::reverse(caminho.begin(), caminho.end());

    int custoCaminho = distancias[destino];

    return std::make_pair(custoCaminho, caminho);
}

void Grafo::encerra_viagem(std::vector<int>& solucao, Vertice& no_ref){
    for(int i = 1;  i<no_ref.path_to_last_destiny.size(); i++){
      solucao.push_back(no_ref.path_to_last_destiny[i]);
      this->nos[no_ref.path_to_last_destiny[i]].visitado = true;
    }      
}

void Grafo::encerra_dia(std::vector<int>& solucao, Vertice& no_ref){
    for(int i = 1;  i<no_ref.path_nearest_hotel.size(); i++){
      solucao.push_back(no_ref.path_nearest_hotel[i]);
      this->nos[no_ref.path_nearest_hotel[i]].visitado = true;
    }
}

std::pair<std::vector<int>, int> Grafo::geraSolucao(double alpha){
    
    // Inicializações
    int origem = this->comeco_fim.first;
    int destino_final = this->comeco_fim.second;
    std::vector<int> solucao;
    bool fim_viagem = false;
    int dias_decorridos = 0;
    float tempo_restante = this->tempos_limites_dias[dias_decorridos];
    Vertice& no_ref = nos[origem];
    std::vector<std::pair<int, float>> candidatos(ordem);
    std::vector<int> pesos_arestas(ordem);
    int pos_candidato = 0;

    // Começo do loop
    while (!fim_viagem) {

        // Permite expansão ocasional do alpha na busca por candidatos
        float alpha_local = alpha;

        // Limpa a lista de candidatos a cada iteração
        candidatos.clear();
        pesos_arestas.clear();

        // Preenche a lista de candidatos
        for (auto& [id_vizinho, peso_aresta] : no_ref.arestas) {
            Vertice& vizinho = nos[id_vizinho];
            float pontuacao_candidato = (vizinho.peso) / peso_aresta;
            candidatos.push_back(std::make_pair(id_vizinho, pontuacao_candidato));
            pesos_arestas.push_back(peso_aresta);
        }

        // Ordena os candidatos em ordem decrescente
        std::sort(candidatos.begin(), candidatos.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second > b.second;
            }
        );

        // Sorteia um candidato
        auto candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));

        // Último dia
        if (dias_decorridos + 1 == this->tam_trip) {
            // Candidato válido
            if (!nos[candidatos[candidato_escolhido].first].visitado && no_ref.cost_last_destiny + pesos_arestas[candidato_escolhido] <= tempo_restante){
                
                if(!no_ref.visitado){
                  solucao.push_back(no_ref.id);
                  no_ref.visitado = true;

                }
                no_ref = nos[candidatos[candidato_escolhido].first];
                // Pega o peso da aresta e subtrai do tempo restante
                tempo_restante -= pesos_arestas[candidato_escolhido];

            } else {
                // Se não achar algum candidato de primeira
                bool buscando_candidato = true;

                while (alpha_local < 0.6 && buscando_candidato) {
                    // Sorteia um candidato
                    candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));

                    if (!nos[candidatos[candidato_escolhido].first].visitado && no_ref.cost_last_destiny + pesos_arestas[candidato_escolhido] <= tempo_restante) {
                        if(!no_ref.visitado){
                          solucao.push_back(no_ref.id);
                          no_ref.visitado = true;

                        }
                        no_ref = nos[candidatos[candidato_escolhido].first];
                        // Pega o peso da aresta e subtrai do tempo restante
                        tempo_restante -= pesos_arestas[candidato_escolhido];
                        buscando_candidato = false;
                    } else {
                        alpha_local += 0.05;
                    }
                }

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (alpha_local >= 0.6) {
                    encerra_viagem(solucao, no_ref);
                    fim_viagem = true;
                }
            }
        } else {
            // Não está no último dia
            // Candidato válido
            if (!nos[candidatos[candidato_escolhido].first].visitado && nos[candidatos[candidato_escolhido].first].cost_nearest_hotel + pesos_arestas[candidato_escolhido] <= tempo_restante) {
                if(!no_ref.visitado){
                  solucao.push_back(no_ref.id);
                  no_ref.visitado = true;
                }
                no_ref = nos[candidatos[candidato_escolhido].first];
                // Pega o peso da aresta e subtrai do tempo restante
                tempo_restante -= pesos_arestas[candidato_escolhido];
            } else {
                // Se não achar algum candidato de primeira
                bool buscando_candidato = true;

                while (alpha_local < 0.6 && buscando_candidato) {
                    // Sorteia um candidato
                    candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));

                    if (!nos[candidatos[candidato_escolhido].first].visitado && nos[candidatos[candidato_escolhido].first].cost_nearest_hotel + pesos_arestas[candidato_escolhido] <= tempo_restante) {
                        if(!no_ref.visitado){
                          solucao.push_back(no_ref.id);
                          no_ref.visitado = true;

                        }
                        no_ref = nos[candidatos[candidato_escolhido].first];
                        // Pega o peso da aresta e subtrai do tempo restante
                        tempo_restante -= pesos_arestas[candidato_escolhido];
                        buscando_candidato = false;
                    } else {
                        alpha_local += 0.05;
                    }
                }

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (alpha_local >= 0.6) {
                    encerra_dia(solucao, no_ref);
                    ++dias_decorridos;
                    tempo_restante = this->tempos_limites_dias[dias_decorridos];
                    // Atualiza o no_ref para partir do hotel onde parou no dia anterior
                    no_ref = nos[solucao[solucao.size() - 1]];
                }
            }
        }
    }

    int pontuacao_solucao = 0;
    for (auto& elemento : solucao)
        pontuacao_solucao += nos[elemento].peso;

    return std::make_pair(solucao, pontuacao_solucao);
}



