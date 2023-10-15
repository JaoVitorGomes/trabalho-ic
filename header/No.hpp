#ifndef NO_HPP
#define NO_HPP

#include <forward_list>
#include <vector>

struct Aresta {
  int id{};
  int peso{ 1 };
};

struct Vertice{

    int id;
    bool visitado = false;
    int peso;
    std::forward_list<Aresta> arestas{};
    bool is_hotel = false;
    std::vector<int> path_nearest_hotel;
    int cost_nearest_hotel;
    std::vector<int> path_to_last_destiny;
    int cost_last_destiny;
    std::pair<int,int> coord;
};


#endif