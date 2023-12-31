#include <algorithm>
#include <iostream>
#include <vector>
#include "../header/Grafo.hpp"
#include "../src/vns.cpp"
#include <fstream>

int main(int argc, const char* argv[])
{   
    std::vector<std::string> instancias;
        
    instancias.push_back("../instances/SET1-1-2/64-45-1-2.ophs");
    instancias.push_back("../instances/SET1-1-2/100-35-1-2.ophs");

    // instancias.push_back("../instances/SET5-12-6/66-130-12-6.ophs");
    // instancias.push_back("../instances/SET5-12-6/100-240-12-6.ophs");

    // instancias.push_back("../instances/SET5-15-4/64-80-15-4.ophs");  
    // instancias.push_back("../instances/SET5-15-4/100-90-15-4.ophs");

    // instancias.push_back("../instances/SET5-15-5/66-130-15-5.ophs");
    // instancias.push_back("../instances/SET5-15-5/100-170-15-5.ophs");

    // instancias.push_back("../instances/SET5-15-6/100-110-15-6.ophs");
    // instancias.push_back("../instances/SET5-15-6/100-200-15-6.ophs");

    // instancias.push_back("../instances/SET5-15-8/100-120-15-8.ophs");
    // instancias.push_back("../instances/SET5-15-8/100-240-15-8.ophs");

    // instancias.push_back("../instances/SET5-15-10/100-150-15-10.ophs");
    // instancias.push_back("../instances/SET5-15-10/100-190-15-10.ophs");

    //instancias.push_back("../instances/SET1-2-3/66-125-2-3.ophs");
    //instancias.push_back("../instances/SET1-2-3/102-60-2-3.ophs");

    //instancias.push_back("../instances/SET1-3-4/64-70-3-4.ophs");
    //instancias.push_back("../instances/SET1-3-4/100-40-3-4.ophs");

    //instancias.push_back("../instances/SET2-5-3/66-60-5-3.ophs");
    //instancias.push_back("../instances/SET2-5-3/100-45-5-3.ophs");

    //instancias.push_back("../instances/SET2-6-4/66-50-6-4.ophs");
    //instancias.push_back("../instances/SET2-6-4/100-35-6-4.ophs");

    // // Útimas 5 instâncias

     //instancias.push_back("../instances/SET3-10-4/66-125-10-4.ophs");
     //instancias.push_back("../instances/SET3-10-4/100-200-10-4.ophs"); 

     //instancias.push_back("../instances/SET3-12-5/64-75-12-5.ophs");
     //instancias.push_back("../instances/SET3-12-5/100-180-12-5.ophs");     

     //instancias.push_back("../instances/SET5-10-5/66-130-10-5.ophs");
     //instancias.push_back("../instances/SET5-10-5/100-80-10-5.ophs"); 

     //instancias.push_back("../instances/SET5-10-6/66-129-10-6.ophs");
     //instancias.push_back("../instances/SET5-10-6/100-110-10-6.ophs"); 

     //instancias.push_back("../instances/SET5-12-4/100-50-12-4.ophs"); 
     //instancias.push_back("../instances/SET5-12-4/100-130-12-4.ophs");
    
    int cyberpunk = 1;
    int set = 1;
    int id_instancia = 1;
    std::vector<int> custosIniciais, custosFinais;
    for(auto instancia : instancias){

        Grafo grafo = Grafo(instancia);
        Grafo copiaGrafo = grafo;
        vector<int> custos;

        //auto [solucao, custo_solucao] = grafo.geraSolucaoZetsubou();

        //custosIniciais.push_back(custo_solucao);

        for (int i = 0; i < 10; i++)
        {
            copiaGrafo = grafo;

            auto solucao = copiaGrafo.geraSolucao(0.1, cyberpunk);
            
            std::cout<<"\n Solucao QUE CHEGOU: \n";
            std::cout<<"[ ";
            for(auto elemento : solucao)
            std::cout<<elemento<<" ";
            std::cout<<" ]\n";

            custosIniciais.push_back(grafo.calculaCusto(solucao));
            
            std::cout<<"\n\nVou chamar VNS\n";
            auto [solucaoVNS, custo_final] = VNS(grafo,grafo.quantidadeNos(),10,solucao);
            std::cout<<"CHAMEI VNS\n";

            custosFinais.push_back(custo_final);
            custos.push_back(custo_final);

        }
        for(auto& cust : custos){
            cout << "valor do custo: " << cust << endl;

        }



        cyberpunk++;

        // Abra o arquivo para escrita
        std::ofstream arquivo("../outputs/SET" + std::to_string(set) + "instancia" + std::to_string(id_instancia) + ".txt");

        // Verifique se o arquivo foi aberto com sucesso
        if (arquivo.is_open())
        {
            // Escreva no arquivo
            for(auto custo : custosIniciais)
                arquivo << custo << " ";

            arquivo << "\n";

            // Escreva no arquivo
            for(auto custo : custosFinais)
                arquivo << custo << " ";


            // Feche o arquivo após a escrita
            arquivo.close();

        }
        else
            std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;

        custosIniciais.clear();
        custosFinais.clear();
        // Atualiza a instância tratada
        id_instancia++;
        
        // Controla quando o set reseta
        if(id_instancia==3){
            std::cout<<"\nTerminei o SET"<<set<<"\n";
            set++;
            id_instancia = 1;
            
        }
        
    } 

    return 0;

}

