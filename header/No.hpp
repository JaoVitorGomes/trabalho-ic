#ifndef NO_HPP
#define NO_HPP

#include <forward_list>
#include <vector>

struct Aresta {
  int id{};
  float peso{ 1.0 };
};

struct Vertice{

    int id;
    bool visitado = false;
    int peso;
    std::forward_list<Aresta> arestas{};
    bool is_hotel = false;
    float cost_nearest_hotel;
    float cost_last_destiny;
    int nearest_hotel;
    std::pair<float,float> coord;
};


#endif