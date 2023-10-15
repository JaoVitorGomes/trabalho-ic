#ifndef TRAB_INTELIGENCIA_COMPUTACIOANAL_HPP
#define TRAB_INTELIGENCIA_COMPUTACIOANAL_HPP

#include "No.hpp"
#include <array>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

class Grafo final {
public:
  Grafo(bool direcionado, std::size_t num_nos = 0, std::size_t num_arestas = 0);
  void adicionarNo(int id, Vertice vertice);                              // b
  void adicionarAresta(int origem, int destino, int peso = 1);     // b
  bool removerNo(int id);
  void removerAresta(int origem, int destino);                     // b
  Vertice& no(int id);                                                  // c
  std::pair<int, std::vector<int>> dijkstra(int origem, int destino);                      // m
  std::pair<std::vector<int>, int> geraSolucao(int origem, double limite_tempo, int destino_final, int num_dias, double alpha);
  void resolveProblema_do_Hotel(std::vector<int> destinos_obrigatorios);
  Grafo* criaGrafoArquivo(std::string filename);     
  
private:
  std::unordered_map<int, Vertice> nos{};
  std::vector<std::vector<int>> floyd_interno();
  int ordem;
  
};

#endif