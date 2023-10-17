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
  Grafo(std::string filename);
  void adicionarNo(int id, Vertice vertice);                              // b
  void adicionarAresta(int origem, int destino, float peso = 1.0);     // b
  bool removerNo(int id);
  void removerAresta(int origem, int destino);                     // b
  void acha_hotel_mais_perto(Vertice& no);
  void acha_caminho_mais_proximo_final_hotel(Vertice& no);
  bool verificarNoRemovidoOuArestaApontaPara(int id);
  Vertice& no(int id);                                      // m
  std::vector<int> geraSolucao(float alpha, int instancia);
  std::pair<std::vector<int>, int> geraSolucaoZetsubou();
  void resolveProblema_do_Hotel(std::vector<int> destinos_obrigatorios); 
  void printa_arestas();   
  bool validarSolucao(std::vector<int> solucao);
  int calculaCusto(std::vector<int> solucao);
  int quantidadeNos();
  bool noHotel(int id);
  int tamanhoTrip();
  int calculaCustoTempo(std::vector<int> solucao);
  std::unordered_map<int, Vertice> copiarNos() {
    return nos;
  }



private:
  std::unordered_map<int, Vertice> nos{};
  std::vector<std::vector<int>> floyd_interno();
  int ordem;
  std::vector<float> tempos_limites_dias;
  std::pair<int, int> comeco_fim;
  std::vector<int> ids_hoteis;
  int tam_trip;
  
};

#endif