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


Grafo::Grafo(bool direcionado, std::size_t num_nos, std::size_t num_arestas)
{
  nos.reserve(num_nos);
}

void Grafo::adicionarNo(int id, Vertice vertice)
{
  nos.emplace(id, vertice);
}

void Grafo::adicionarAresta(int origem, int destino, int peso)
{
  auto itOrigem = nos.find(origem);
  auto itDestino = nos.find(destino);
  if (itOrigem == nos.end() || itDestino == nos.end()) {
    return;
  }
  Aresta aresta{ destino, peso };
  itOrigem->second.arestas.push_front(aresta);
  Aresta arestaOposta{ origem, peso };
  itDestino->second.arestas.push_front(arestaOposta);
  
}

bool Grafo::removerNo(int id)
{
    return true;
}

void Grafo::removerAresta(int origem, int destino)
{
  auto itOrigem = nos.find(origem);
  auto itDestino = nos.find(destino);
  if (itOrigem == nos.end() || itDestino == nos.end()) {
    return;
  }

  auto& arestas = itOrigem->second.arestas;
  auto numRemovidos = std::count_if(arestas.begin(), arestas.end(), [destino](const Aresta& aresta) { return aresta.id == destino; });

  if (numRemovidos == 0)
    return;
  
  itDestino->second.arestas.remove_if([origem](const Aresta& aresta) { return aresta.id == origem; });
  
}

Vertice& Grafo::no(int id)
{
  return nos.at(id);
}

std::pair<int, std::vector<int>> Grafo::dijkstra(int origem, int destino) {
    int ordem = nos.size();
    auto itOrigem = nos.find(origem);
    auto itDestino = nos.find(destino);
    if (ordem <= 1)
        return std::make_pair(0, std::vector<int>());

    Vertice& noOrigem = itOrigem->second;
    Vertice& noDestino = itDestino->second;

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


std::vector<std::vector<int>> Grafo::floyd_interno()
{
  // Criam -se as matrizes
  std::vector<std::vector<int>> distancias(nos.size() + 1, std::vector<int>(nos.size() + 1, std::numeric_limits<int>::max()));
  std::vector<std::vector<int>> proximo(nos.size() + 1, std::vector<int>(nos.size() + 1, -1));

  // Inicializa as distâncias e próximos nós com base nas arestas existentes
  for (const auto& [id, no] : nos) {
    distancias[id][id] = 0; // A distância de um nó para si mesmo é zero

    for (const auto& aresta : no.arestas) {
      distancias[id][aresta.id] = aresta.peso; // Supondo peso 1 para as arestas
      proximo[id][aresta.id] = aresta.id;
    }
  }

  // Percorre todos os nós para encontrar os caminhos mínimos
  for (int k = 0; k < nos.size(); ++k) {
    for (int i = 0; i < nos.size(); ++i) {
      for (int j = 0; j < nos.size(); ++j) {
        if (distancias[i][k] != std::numeric_limits<int>::max() && distancias[k][j] != std::numeric_limits<int>::max() && distancias[i][k] + distancias[k][j] < distancias[i][j]) {
          distancias[i][j] = distancias[i][k] + distancias[k][j];
          proximo[i][j] = proximo[i][k];
        }
      }
    }
  }
  return distancias;
}

void encerra_viagem(std::vector<int>& solucao, std::vector<int>& caminhoFinal){
    for(auto elemento : caminhoFinal)
    solucao.push_back(elemento);

}

void encerra_dia(std::vector<int>& solucao, Vertice& no_ref){
  for(auto elemento : no_ref.path_nearest_hotel)
    solucao.push_back(elemento);

}

std::pair<std::vector<int>, int> Grafo::geraSolucao(int origem, double limite_tempo, int destino_final, int num_dias, double alpha) {

    // INICIALIZÇÕES
    std::vector<int> solucao;
    bool fim_viagem = false;
    std::vector<std::pair<int, float>> candidatos(ordem-1);
    int pos_candidato = 0;
    int dias_decorridos = 0;
    float tempo_restante = limite_tempo;
    Vertice& no_ref = nos[origem];

    // COMEÇO LOOP
    while(!fim_viagem){

      // Permite a expansão ocasional do alpha na busca por candidatos
      float alpha_local = alpha;

      // Preenche a lista de candidatos
      for(auto& [id_vizinho, peso_aresta] : no_ref.arestas){
          Vertice& vizinho = nos[id_vizinho];
          float pontuacao_candidato = (vizinho.peso)/peso_aresta;
          candidatos[pos_candidato] = std::make_pair(id_vizinho, pontuacao_candidato); 
          ++pos_candidato;
      }

      pos_candidato = 0;

      // Ordena os candidatos em ordem decrescente  
      std::sort(candidatos.begin(), candidatos.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second > b.second;
            }
      );

      // Sorteia um candidato
      auto candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha * (ordem - 1))));
      
      // üLTIMO DIA 
      if (dias_decorridos+1 == num_dias){

        // Candidato válido
        if(!nos[candidatos[candidato_escolhido].first].visitado && nos[candidato_escolhido].cost_last_destiny <= tempo_restante){
                solucao.push_back(no_ref.id);
                no_ref.visitado = true;
                no_ref = nos[candidato_escolhido];
                // Pega o peso da aresta e subtrai do tempo restante
                tempo_restante-=nos[candidatos[candidato_escolhido].first].peso * candidatos[candidato_escolhido].second;
            }
        // Se não achar algum candidato de primeira
        else{
  
              bool buscando_candidato = true;
                
              while (alpha_local<0.6 && !buscando_candidato){
                  
                // Sorteia um candidato
                auto candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * (ordem - 1))));

                if(!nos[candidatos[candidato_escolhido].first].visitado && nos[candidato_escolhido].cost_last_destiny <= tempo_restante){
                    solucao.push_back(no_ref.id);
                    no_ref.visitado = true;
                    no_ref = nos[candidato_escolhido];
                    // Pega o peso da aresta e subtrai do tempo restante
                    tempo_restante-=nos[candidatos[candidato_escolhido].first].peso * candidatos[candidato_escolhido].second;
                    buscando_candidato = false;
                }
                // Se novamente não achar ninguém, aumenta o escopo
                else
                  alpha_local+=0.05;

              }
              // Se mesno aumentando tanto assim o escopo não achar ninguém, encerra o dia
              if(alpha_local==0.6){
                encerra_viagem(solucao, no_ref.path_to_last_destiny);
                fim_viagem = true;
              }    
          }
      }
      
      // NÃO ESTÁ NO ÚLTIMO DIA
      else{
            if(!nos[candidatos[candidato_escolhido].first].visitado && nos[candidatos[candidato_escolhido].first].cost_nearest_hotel <= tempo_restante){
                solucao.push_back(no_ref.id);
                no_ref.visitado = true;
                no_ref = nos[candidato_escolhido];
                // Pega o peso da aresta e subtrai do tempo restante
                tempo_restante-=nos[candidatos[candidato_escolhido].first].peso * candidatos[candidato_escolhido].second;
            }

            // Se não achar algum candidato de primeira
            else{
                
                bool buscando_candidato = true;
                
                while (alpha_local<0.6 && !buscando_candidato){
                  
                  // Sorteia um candidato
                  auto candidato_escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * (ordem - 1))));

                  if(!nos[candidatos[candidato_escolhido].first].visitado && nos[candidatos[candidato_escolhido].first].cost_nearest_hotel <= tempo_restante){
                      solucao.push_back(no_ref.id);
                      no_ref.visitado = true;
                      no_ref = nos[candidato_escolhido];
                      // Pega o peso da aresta e subtrai do tempo restante
                      tempo_restante-=nos[candidatos[candidato_escolhido].first].peso * candidatos[candidato_escolhido].second;
                      buscando_candidato = false;
                  }
                  // Se novamente não achar ninguém, aumenta o escopo
                  else
                    alpha_local+=0.05;

                }
                // Se mesno aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if(alpha_local==0.6){
                  encerra_dia(solucao, no_ref);
                  ++dias_decorridos;
                  tempo_restante = limite_tempo;
                  // Atualiza o no_ref para partir do hotel onde se parou no dia anterior
                  no_ref = nos[solucao[solucao.size()-1]];
                }    
            }
        }
    } 
    
    int pontuacao_solucao = 0;
    for(auto& elemento : solucao)
      pontuacao_solucao+=nos[elemento].peso;

    return std::make_pair(solucao, pontuacao_solucao);   
}

Grafo* Grafo::criaGrafoArquivo(std::string filename) {
    std::ifstream file(filename);
    std::string line, linha1,linha2,linha3;

    int V = 0;
    int vert, hotEx, trip;
    std::vector<std::pair<int, int>> edges;
    std::vector<int> pesos;

    getline(file, line);
    linha1 = line;
    std::istringstream iss(line);
    iss >> vert >> hotEx >> trip;

    getline(file, line);
    linha2 = line;

    getline(file, line);
    linha3 = line;

    getline(file,line);

    while (getline(file, line)) {

      std::istringstream iss(line);
      int x, y , si;
      iss >> x >> y >> si;
      edges.push_back(std::make_pair(x, y));
      pesos.push_back(si);
      V = std::max(V, std::max(x, y));

    }

    Grafo* grafo = new Grafo(false,vert,((vert * vert-1)/2));
    int cout = 0;

    for (auto no : edges) {

      Vertice novono;
      novono.id = cout;
      novono.coord = no;
      novono.peso = pesos[cout];

      grafo->adicionarNo(cout,novono);
      cout++;

    }

    return grafo;


}


