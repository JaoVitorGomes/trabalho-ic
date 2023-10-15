#include <algorithm>
#include <iostream>
#include <vector>
#include "../header/Grafo.hpp"
#include "../src/vns.cpp"
using namespace std;

int main(int argc, const char* argv[])
{   

    Grafo teste = Grafo("../instances/SET1 1-2/64-45-1-2.ophs");
    auto [solucao, custo_solucao] = teste.geraSolucao(0.05);
    std::cout<<"\nCusto solucao inicial: "<<custo_solucao;
    std::cout<<"\nSolucao inicial:\n"<<solucao[0]<<"->";
    for(int i = 1; i<solucao.size(); i++)
        std::cout<<solucao[i]<<"->";

    auto solucaoVNS = VNS(teste,teste.quantidadeNos(),1000);
    std::cout<<"\nSolucao FINAL:\n"<<solucao[0]<<"->";
    for(auto& elemento : solucaoVNS)
        std::cout<<elemento<<"->";
    return 0;

}

