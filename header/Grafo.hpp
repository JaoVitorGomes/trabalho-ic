#ifndef TRAB_INTELIGENCIA_COMPUTACIOANAL_HPP
#define TRAB_INTELIGENCIA_COMPUTACIOANAL_HPP

#include "No.hpp"
#include <array>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Grafo final {
public:
  Grafo(bool direcionado, std::size_t num_nos = 0, std::size_t num_arestas = 0);
  void adicionarNo(int id, Vertice vertice);                              // b
  void adicionarAresta(int origem, int destino, int peso = 1);     // b
  bool removerNo(int id);
  void removerAresta(int origem, int destino);                     // b
  Vertice& no(int id);                                                  // c
  int dijkstra(int origem, int destino);                           // m
  //std::vector<Vertice&> geraSolucaoInicial(std::vector<int> destinos_obrigatorios, double alpha);
  void resolveProblema_do_Hotel(std::vector<int> destinos_obrigatorios);
  
private:
  std::unordered_map<int, Vertice> nos{};
  std::unordered_map<int, Vertice> hoteis{};
  std::vector<std::vector<int>> floyd_interno();
  
};

#endif