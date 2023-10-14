#ifndef NO_HPP
#define NO_HPP

#include <forward_list>


struct Aresta {
  int id{};
  int peso{ 1 };
};

struct Vertice{

    int id;
    int hotel_maix_proximo;
    bool visitado;
    int peso;
    std::forward_list<Aresta> arestas{};
    bool obrigatorio = false;
};


#endif