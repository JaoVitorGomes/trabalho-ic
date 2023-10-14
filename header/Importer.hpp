#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include "grafo.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
/*
auto import(const std::string& file_path, bool direcionado) -> std::optional<Grafo>
{
  auto file = std::ifstream{ file_path };
  if (!file) {
    std::cerr << "Could not open file " << file_path << '\n';
    return {};
  }

  auto num_nodes = std::size_t{};
  auto num_edges = std::size_t{};
  file >> num_nodes >> num_edges;
  std::cout << "Opening file " << file_path << "\n\t" << num_nodes << " nodes\n\t" << num_edges << " edges\n";

  auto grafo = Grafo{ direcionado, num_nodes, num_edges };

  while (file) {
    file.ignore(std::numeric_limits<std::streamsize>::max(), 'e'); // discards "\n" and the next "e"
    auto node_id_a = int{};
    auto node_id_b = int{};
    file >> node_id_a >> node_id_b; // testar se node_id_a e node_id_b estao com os valores corretos (usar debugger)

    auto node_weight_a = (node_id_a % 200) + 1;
    grafo.adicionarNo(node_id_a, node_weight_a);

    auto node_weight_b = (node_id_b % 200) + 1;
    grafo.adicionarNo(node_id_b, node_weight_b);

    grafo.adicionarAresta(node_id_a, node_id_b, 1);
  }

  return grafo;
}
*/

#endif