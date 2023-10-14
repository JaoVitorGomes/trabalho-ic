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

Grafo::Grafo(bool direcionado, std::size_t num_nos, std::size_t num_arestas)
{
  nos.reserve(num_nos);
}

void Grafo::adicionarNo(int id, Vertice vertice)
{
  nos.emplace(id, vertice);
}

bool Grafo::removerNo(int id)
{
    return true;
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

// Testa se realmente está pegando o peso das arestas
int Grafo::dijkstra(int origem, int destino)
{
  int ordem = nos.size();
  auto itOrigem = nos.find(origem);
  auto itDestino = nos.find(destino);
  if (ordem <= 1)
    return 0;

  Vertice& noOrigem = itOrigem->second;
  Vertice& noDestino = itDestino->second;

  std::unordered_map<int, int> distancias;
  for (const auto& par : nos)
    distancias[par.first] = std::numeric_limits<int>::max();

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
        fila.push(std::make_pair(novaDistancia, idAdjacente));
      }
    }
  }
  std::cout << "Custo do caminho minimo usando djikstra: " << distancias[destino] << "\n";
  return distancias[destino];
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

/*std::vector<Vertice&> geraSolucaoInicial(std::vector<int> destinos_obrigatorios, double alpha);
{

  auto melhor_solucao = Cobertura{};
  auto melhor_custo = Custo{ std::numeric_limits<int>::max() };

  using Id = int;
  auto start_nos = std::unordered_map<Id, Vertice>{};
  start_nos.reserve(ordem());
  for (const auto& [id, no] : nos) {
    auto [it, _] = start_nos.emplace(std::make_pair(id, NoCobertura{ no.peso, no.peso, no.grau_entrada }));
    for (const auto& aresta : no.arestas) {
      it->second.delta -= nos.at(aresta.id).peso;
    }
  }

  std::vector<Id> start_ids_ordenados;
  for (const auto& pair : start_nos) {
    start_ids_ordenados.push_back(pair.first);
  }
  std::ranges::sort(start_ids_ordenados, [&](const auto& a, const auto& b) {
    return start_nos[a].delta < start_nos[b].delta;
  });

  for (auto count = 0; count < 1000; ++count) {
    auto nos = start_nos;
    auto ids_ordenados = start_ids_ordenados;
    auto solucao = Cobertura{};
    auto custo = Custo{ 0 };
    auto num_arestas_cobertas = int{ 0 };

    while (num_arestas_cobertas < m_num_arestas) {
      auto index = Random::get(0, static_cast<int>(std::ceil(alpha * (nos.size() - 1))));
      auto id = ids_ordenados[index];
      auto min = nos[id];
      nos.erase(id);
      ids_ordenados.erase(ids_ordenados.begin() + index);

      if (min.grau > 0) {
        solucao.push_back(id);
        custo += min.peso;
        num_arestas_cobertas += min.grau;

        for (const auto& aresta : m_nos[id].arestas) {
          if (auto vizinho = nos.find(aresta.id); vizinho != nos.end()) {
            vizinho->second.delta += min.peso;
            vizinho->second.grau -= 1;
          }
        }
      }

      std::ranges::sort(ids_ordenados, [&](auto a, auto b) {
        return nos[a].delta < nos[b].delta;
      });
    }

    if (custo < melhor_custo) {
      melhor_custo = custo;
      melhor_solucao = solucao;
    }
  }

  return { melhor_solucao, melhor_custo };
}*/



