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
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>

Grafo::Grafo(std::string filename){

    std::ifstream file(filename);
    std::string line, linha1,linha2,linha3;

    int num_vertices, num_hoteis_extras, numero_dias;
    std::vector<std::pair<float, float>> coordenadas_nos;
    std::vector<int> pesos;

    // Pega: número de vértice + 2, número de hoteis extra, número de trips = número de dias
    getline(file, line);
    linha1 = line;
    std::istringstream conversor(line);
    conversor >> num_vertices >> num_hoteis_extras >> numero_dias;
    
    // Tempo total
    getline(file, line);
    linha2 = line;

    // Tamanho para as trips = tempos limites de cada trip/dia 
    getline(file, line);
    linha3 = line;

    // Pegando o tempo limite de cada dia
    std::istringstream conversorLinha3(linha3);
    std::string token;
    
    while (std::getline(conversorLinha3, token, '\t'))
        this->tempos_limites_dias.push_back(std::stof(token));

    // Pega linha em branco
    getline(file,line);

    std::vector<float> dadosNo;

    // Adicionando os nós
    while (getline(file, line)) {
      std::istringstream conversor(line);
      while (std::getline(conversor, token, '\t'))
        dadosNo.push_back(std::stof(token));
      coordenadas_nos.push_back(std::make_pair(dadosNo[0], dadosNo[1]));
      pesos.push_back(dadosNo[2]);
      dadosNo.clear();

    }

    int contador = 0;

    for (auto no : coordenadas_nos) {

      Vertice novono;
      novono.id = contador;
      novono.coord = no;
      novono.peso = pesos[contador];

      if (novono.peso == 0){
        novono.is_hotel = true;
        this->ids_hoteis.push_back(novono.id);
      }

      this->adicionarNo(contador,novono);
      contador++;

    }

    this->ordem = num_vertices;
    this->comeco_fim = std::make_pair(0, 1);
    this->nos.reserve(this->ordem);
    this->tam_trip = numero_dias;

    // CADASTRANDO ARESTAS
    for ( auto noExterno : this->nos){
        for ( auto noInterno : this->nos){
          float pesoAresta = std::sqrt( std::pow((noExterno.second.coord.first - noInterno.second.coord.first), 2) + std::pow((noExterno.second.coord.second - noInterno.second.coord.second), 2) ); 
          this->adicionarAresta(noExterno.first, noInterno.first, pesoAresta);
        }
            
    }

    // Preenche a distância mais próxima até um hotel
    for(auto& elemento : this->nos)
      this->acha_hotel_mais_perto(elemento.second);
    
    // Preenche a distância mais próxima até o destino final
    for(auto& elemento : this->nos)
      this->acha_caminho_mais_proximo_final_hotel(elemento.second);

    // TESTES
    /*
      // Printando hotel
      for(int i=0; i<this->nos.size(); i++){

        std::cout << "\nNo: "<<this->nos[i].id<< " Custo para o hotel mais proximo: "<<this->nos[i].cost_nearest_hotel;
        // std::cout<<"\nCaminho para o hotel mais proximo:\n";

        //for(auto& elemento_caminho : elemento.second.path_nearest_hotel)
          //std::cout<<elemento_caminho << "->";

      }
      
      for(auto& elemento : this->ids_hoteis)
        std::cout<<"\nId hotel: "<<elemento;

      //std::cout<<"\nNumero de nos: "<< this->ordem;
      ids_hoteis
      //this->printa_arestas();
    */
   return;
}

void Grafo::adicionarNo(int id, Vertice vertice)
{
  nos.emplace(id, vertice);
}

void Grafo::acha_hotel_mais_perto(Vertice& no){

  std::vector<std::pair<int, float>> custos_hoteis;
  for(auto hotel : this->ids_hoteis){
    auto custo = std::sqrt( std::pow((no.coord.first - nos[hotel].coord.first), 2) + std::pow((no.coord.second - nos[hotel].coord.second), 2) );
    custos_hoteis.push_back(std::make_pair(hotel, custo));
  }
  // Ordeno custos_hoteis com base no custo em ordem crescente
  int nearest = custos_hoteis[0].first;
  float menor_custo = custos_hoteis[1].second;
  for(auto [id, custo] : custos_hoteis)
    if(menor_custo>custo){
      nearest = id;
      menor_custo = custo;
    }
  no.nearest_hotel = nearest;
  no.cost_nearest_hotel;

}

void Grafo::acha_caminho_mais_proximo_final_hotel(Vertice& no){

    no.cost_last_destiny = std::sqrt( std::pow((no.coord.first - nos[1].coord.first), 2) + std::pow((no.coord.second - nos[1].coord.second), 2) );

}
void Grafo::printa_arestas() {
    for (const auto& pair : nos) {
        const Vertice& vertice = pair.second;
        std::cout << "\nArestas do Vértice " << vertice.id << ":\n";
        
        for (const Aresta& aresta : vertice.arestas) {
            std::cout << "  Destino: " << aresta.id << ", Peso: " << aresta.peso << "\n";
        }
    }
}


void Grafo::adicionarAresta(int origem, int destino, float peso)
{
  auto noOrigem = nos.find(origem);
  auto noDestino = nos.find(destino);
  if (noOrigem == nos.end() || noDestino == nos.end()) {
    return;
  }
  Aresta aresta{ destino, peso};
  noOrigem->second.arestas.push_front(aresta);
  
}

bool Grafo::removerNo(int id) {

      // Crie uma função lambda que verifica se a aresta deve ser removida
      auto shouldRemoveAresta = [id](const Aresta& aresta) { return aresta.id == id; };

      // Itera sobre todos os nós do grafo
      for (auto& [_, no] : nos) {
          // Use a função std::remove_if seguida de no.arestas.erase para remover as arestas que atendem à condição
          no.arestas.remove_if(shouldRemoveAresta);
      }
  

    nos.erase(id);

    this->ordem = this->ordem - 1;

    return true;
}

void Grafo::removerAresta(int origem, int destino)
{
  auto noOrigem = nos.find(origem);
  auto noDestino = nos.find(destino);
  if (noOrigem == nos.end() || noDestino == nos.end()) {
    return;
  }

  auto& arestas = noOrigem->second.arestas;
  auto numRemovidos = std::count_if(arestas.begin(), arestas.end(), [destino](const Aresta& aresta) { return aresta.id == destino; });

  if (numRemovidos == 0)
    return;
  
  noDestino->second.arestas.remove_if([origem](const Aresta& aresta) { return aresta.id == origem; });
  
}

Vertice& Grafo::no(int id){
  return nos.at(id);
}

std::vector<int> Grafo::geraSolucao(float alpha, int instancia){
    
    // Inicializações
    std::vector<int> solucao;

    int origem = this->comeco_fim.first;
    int destino_final = this->comeco_fim.second;
    bool fim_viagem = false;
    int dias_decorridos = 0;

    float tempo_restante = this->tempos_limites_dias[dias_decorridos];
    Vertice& no_ref = nos[origem];

    // Começo do loop
    while (!fim_viagem) {

        std::cout<<"\nNo de referencia: "<<no_ref.id<<" Arestas do no de referencia:\n";
        
        for(auto aresta : no_ref.arestas)
          std::cout<<aresta.id<<"->";

        std::cout<<"\n";

        std::cout<<"\n Solucao ate o momento: \n";
        std::cout<<"[ ";
        for(auto elemento : solucao)
          std::cout<<elemento<<" ";
        std::cout<<" ]\n";

        // Permite expansão ocasional do alpha na busca por candidatos
        float alpha_local = alpha;

        // Limpa a lista de candidatos a cada iteração
        std::vector<std::pair<int, float>> candidatos;
        std::vector<float> pesos_arestas;

        // Se não houver arestas saindo do nó de referência termina a viagem
        if (no_ref.arestas.empty()){
            fim_viagem = true;
            continue;
        }
        

        // Preenche a lista de candidatos
        for (auto [id_vizinho, peso_aresta] : no_ref.arestas) {
            Vertice& vizinho = nos[id_vizinho];
            if(id_vizinho==no_ref.id || nos[id_vizinho].visitado)
              continue;
            float pontuacao_candidato = (vizinho.peso) / peso_aresta;
            candidatos.push_back(std::make_pair(id_vizinho, pontuacao_candidato));
            pesos_arestas.push_back(peso_aresta);
        }

        // Se não houver candidatos, encerra tudo
        if(candidatos.empty()){
          fim_viagem = true;
          continue;
        }

        // Ordena os candidatos em ordem decrescente
        std::sort(candidatos.begin(), candidatos.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second > b.second;
            }
        );
        std::cout<<"\nLista de candidatos ORDENADA: \n";
        std::cout<<"{ ";
        for(auto elemento : candidatos)
          std::cout<<elemento.first<<" ";
        std::cout<<" }\n";

        // Sorteia um candidato
        auto escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));

        std::cout<<"Antes de saber se eh ou nao o ultimo dia: \n";        
        std::cout<<"\nId escolhido em CANDIDATOS: "<<escolhido;
        std::cout<<"\nNO DE FATO ESCOLHIDO: "<<nos[candidatos[escolhido].first].id;

        // ÚLTIMO DIA
        if (dias_decorridos + 1 == this->tam_trip) {

            // Candidato válido
            if (nos[candidatos[escolhido].first].cost_last_destiny + pesos_arestas[escolhido] <= tempo_restante){
                std::cout<<"\nULTIMO DIA IF\n";        
                std::cout<<"\nId escolhido em CANDIDATOS: "<<escolhido;
                std::cout<<"\nNO DE FATO ESCOLHIDO: "<<nos[candidatos[escolhido].first].id;
		            solucao.push_back(no_ref.id);
                no_ref.visitado = true;
                nos[no_ref.id].visitado = true;
                no_ref = nos[candidatos[escolhido].first];
                tempo_restante -= pesos_arestas[escolhido];

            } 
            else {
                // Se não achar algum candidato de primeira
                bool buscando_candidato = true;

                while (alpha_local < 0.6 && buscando_candidato) {
                    
                    escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));

                    std::cout<<"\nULTIMO DIA ELSE\n";        
                    std::cout<<"\nId escolhido em CANDIDATOS: "<<escolhido;
                    std::cout<<"\nNO DE FATO ESCOLHIDO: "<<nos[candidatos[escolhido].first].id;
                    std::cout<<"\nAlpha Local: "<<alpha_local<<"\n";


                    if (nos[candidatos[escolhido].first].cost_last_destiny + pesos_arestas[escolhido] <= tempo_restante) {
                        
                        solucao.push_back(no_ref.id);
                        no_ref.visitado = true;
                        nos[no_ref.id].visitado = true;

                        no_ref = nos[candidatos[escolhido].first];
                        tempo_restante -= pesos_arestas[escolhido];
                        
                        buscando_candidato = false;

                    } 
                    // Aumenta escopo
		                else 
                      alpha_local += 0.05;
                    
                }

                std::cout<<"\nSai do while, alpha local = "<<alpha_local<<std::endl;

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (alpha_local >= 0.6) {
                    std::cout<<"\nVOU MATAR A VIAGEM: Aljpha local = "<<alpha_local<<std::endl;
                    solucao.push_back(no_ref.id);
                    no_ref.visitado = true;
                    nos[no_ref.id].visitado = true;
                    solucao.push_back(1);
                    nos[1].visitado = true;
                    fim_viagem = true;
                    continue;
                }
            }
        } 
        // NÃO É O ÚLTIMO DIA
        else {
            
            if (nos[candidatos[escolhido].first].cost_nearest_hotel + pesos_arestas[escolhido] <= tempo_restante) {
                
                std::cout<<"\nDIA: COMUM IF\n";        
                std::cout<<"\nId escolhido em CANDIDATOS: "<<escolhido;
                std::cout<<"\nNO DE FATO ESCOLHIDO: "<<nos[candidatos[escolhido].first].id;

                // Não coloco hotéis na solução aqui 
                solucao.push_back(no_ref.id);
                no_ref.visitado = true;
                nos[no_ref.id].visitado = true;

                no_ref = nos[candidatos[escolhido].first];
                tempo_restante -= pesos_arestas[escolhido];
    
            } 
	          else {

                // Se não achar algum candidato de primeira
                bool buscando_candidato = true;

                while (alpha_local < 0.6 && buscando_candidato) {
                    // Sorteia um candidato
                    escolhido = Random::get(0, static_cast<int>(std::ceil(alpha_local * candidatos.size()) - 1));
                    
                    std::cout<<"\nDIA: COMUM ELSE\n";        
                    std::cout<<"\nId escolhido em CANDIDATOS: "<<escolhido;
                    std::cout<<"\nNO DE FATO ESCOLHIDO: "<<nos[candidatos[escolhido].first].id;
                    std::cout<<"\nAlpha Local: "<<alpha_local<<"\n";

                    if (nos[candidatos[escolhido].first].cost_nearest_hotel + pesos_arestas[escolhido] <= tempo_restante) {
                      
                      solucao.push_back(no_ref.id);
                      no_ref.visitado = true;
                      nos[no_ref.id].visitado = true;

                      no_ref = nos[candidatos[escolhido].first];
                      tempo_restante -= pesos_arestas[escolhido];
                      
                      buscando_candidato = false;

                    } 
                    else
                        alpha_local += 0.05;
                }

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (alpha_local >= 0.6) {
                    
                    solucao.push_back(no_ref.id);
                    no_ref.visitado = true;
                    nos[no_ref.id].visitado = true;
                    
                    no_ref = this->nos[no_ref.nearest_hotel]; 
                    
                    ++dias_decorridos;
                    tempo_restante = this->tempos_limites_dias[dias_decorridos];

                    std::cout<<"\nMudei de DIA\n";
                    std::cout<<"\nInstancia: "<<instancia;
                    
                }
            }
        }
    }

    std::cout<<"\nNIGERANDAIO SMOKEEEEE"<<std::endl;
    std::cout<<"\n Solucao THE FINAL: \n";
    std::cout<<"[ ";
    for(auto elemento : solucao)
        std::cout<<elemento<<" ";
    std::cout<<" ]\n";
    
    return solucao;

}

bool Grafo::verificarNoRemovidoOuArestaApontaPara(int id) {
    // Verifica se o nó com o ID fornecido ainda existe no grafo

    // Verifica se alguma aresta aponta para o nó com o ID fornecido
    for (auto& par : nos) {
        Vertice& vertice = par.second;
        for (const Aresta& aresta : vertice.arestas) {
            if (aresta.id == id) {
                return true; // Existe uma aresta que aponta para o nó
            }
        }
    }

    // Se o nó não foi encontrado e nenhuma aresta aponta para ele, ele não está mais no grafo
    return false;
}

/*
std::pair<std::vector<int>, int> Grafo::geraSolucaoZetsubou(){

    auto nos_local = this->copiarNos();
    
    // Inicializações
    int origem = this->comeco_fim.first;
    int destino_final = this->comeco_fim.second;
    std::vector<int> solucao;
    std::vector<std::pair<int, float>> candidatos;
    std::vector<float> pesos_arestas;
    bool fim_viagem = false;
    int dias_decorridos = 0;
    float tempo_restante = this->tempos_limites_dias[dias_decorridos];
    Vertice no_ref = nos[origem];

    // Começo do loop
    while (!fim_viagem) {

        // Limpa a lista de candidatos a cada iteração
        candidatos.clear();
        pesos_arestas.clear();

        // Preenche a lista de candidatos
        for (auto [id_vizinho, peso_aresta] : no_ref.arestas) {
            Vertice& vizinho = nos[id_vizinho];
            if(id_vizinho==no_ref.id)
              continue;
            float pontuacao_candidato = (vizinho.peso) / peso_aresta;
            candidatos.push_back(std::make_pair(id_vizinho, pontuacao_candidato));
            pesos_arestas.push_back(peso_aresta);
        }

        // Ordena os candidatos em ordem decrescente
        std::sort(candidatos.begin(), candidatos.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second > b.second;
            }
        );

        int escolhido = candidatos[0].first;
       
        // Último dia
        if (dias_decorridos + 1 == this->tam_trip) {
            // Candidato válido
            if (nos[escolhido].cost_last_destiny + pesos_arestas[0] <= tempo_restante && !nos[escolhido].is_hotel){
                
                solucao.push_back(no_ref.id);
                nos_local[no_ref.id].visitado = true;
                int caraRemovido = no_ref.id;
                if(!no_ref.is_hotel)
                  this->removerNo(no_ref.id);

                if(!this->verificarNoRemovidoOuArestaApontaPara(caraRemovido))
                  std::cout<<"\nRemovi de fato tudo do "<<caraRemovido<<std::endl;
                else
                  std::cout<<"\nNAO Removi de fato tudo do "<<caraRemovido<<std::endl;

                no_ref = nos[escolhido];
                tempo_restante -= pesos_arestas[0];
                this->recalcula_tudo();

            } 
            else {
                int i;
                for (i = 1; i < candidatos.size(); i++){

                    escolhido = candidatos[i].first;

                    if (nos[escolhido].cost_last_destiny + pesos_arestas[i] <= tempo_restante && !nos[escolhido].is_hotel) {
                        
                        solucao.push_back(no_ref.id);
                        nos_local[no_ref.id].visitado = true;
                        int caraRemovido = no_ref.id;
                        if(!no_ref.is_hotel)
                          this->removerNo(no_ref.id);
                        if (!this->verificarNoRemovidoOuArestaApontaPara(caraRemovido))
                          std::cout << "\nRemovi de fato tudo do " << caraRemovido << std::endl;
                        else
                          std::cout << "\nNAO Removi de fato tudo do " << caraRemovido << std::endl;
                        no_ref = nos[escolhido];
                        // Pega o peso da aresta e subtrai do tempo restante
                        tempo_restante -= pesos_arestas[i];
                        this->recalcula_tudo();
                        break;
                    } 
                }

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (i == candidatos.size()) {
                    encerra_viagem(solucao, no_ref);
                    fim_viagem = true;
                }
            }
        } 
        else {
            // Não está no último dia
            // Candidato válido
            if (nos[escolhido].cost_nearest_hotel + pesos_arestas[0] <= tempo_restante && !nos[escolhido].is_hotel) {
                
                solucao.push_back(no_ref.id);
                nos_local[no_ref.id].visitado = true;
                int caraRemovido = no_ref.id;
                if(!no_ref.is_hotel)
                  this->removerNo(no_ref.id);
                if (!this->verificarNoRemovidoOuArestaApontaPara(caraRemovido))
                  std::cout << "\nRemovi de fato tudo do " << caraRemovido << std::endl;
                else
                  std::cout << "\nNAO Removi de fato tudo do " << caraRemovido << std::endl;
                no_ref = nos[escolhido];
                // Pega o peso da aresta e subtrai do tempo restante
                tempo_restante -= pesos_arestas[0];
                this->recalcula_tudo();

            } 
            else {

                int j = 1;
                for(j = 1; j<candidatos.size(); j++){

                    escolhido = candidatos[j].first;

                    if (nos[escolhido].cost_nearest_hotel + pesos_arestas[j] <= tempo_restante && !nos[escolhido].is_hotel) {

                        solucao.push_back(no_ref.id);
                        nos_local[no_ref.id].visitado = true;
                        int caraRemovido = no_ref.id;
                        if(!no_ref.is_hotel)
                          this->removerNo(no_ref.id);
                        if (!this->verificarNoRemovidoOuArestaApontaPara(caraRemovido))
                          std::cout << "\nRemovi de fato tudo do " << caraRemovido << std::endl;
                        else
                          std::cout << "\nNAO Removi de fato tudo do " << caraRemovido << std::endl;
                        no_ref = nos[escolhido];
                        // Pega o peso da aresta e subtrai do tempo restante
                        tempo_restante -= pesos_arestas[j];
                        this->recalcula_tudo();
                        break;

                    }
                }

                // Se mesmo aumentando tanto assim o escopo não achar ninguém, encerra o dia
                if (j == candidatos.size()) {
                    encerra_dia(solucao, no_ref);
                    this->removerNo(no_ref.id);
                    no_ref = this->nos[solucao.at(solucao.size()-1)];
                    dias_decorridos++;
                    tempo_restante = this->tempos_limites_dias[dias_decorridos];
                }
            }
        }
    }

    int pontuacao_solucao = 0;
    this->nos = nos_local;
    for (auto& elemento : solucao)
        pontuacao_solucao += nos[elemento].peso;
 
    return std::make_pair(solucao, pontuacao_solucao);
}
*/
bool Grafo::validarSolucao(std::vector<int> solucao){
  int contador_dias = 0;
  std::vector<float> pesos_arestas;

  // Preenche a lista de candidatos
  for (auto& [id_vizinho, peso_aresta] : this->nos[solucao[0]].arestas)
      if(id_vizinho == solucao[1]){
        pesos_arestas.push_back(peso_aresta);
        break;
      }

  for(int i = 1; i < solucao.size()-2; i++){

    for (auto& [id_vizinho, peso_aresta] : this->nos[solucao[i]].arestas)
      if(id_vizinho == solucao[i+1]){
        pesos_arestas.push_back(peso_aresta);
        break;
    }

    float soma_parcial = 0;
    for(auto& peso : pesos_arestas)
      soma_parcial+=peso;

    if(soma_parcial > this->tempos_limites_dias[contador_dias]){
      return false;
    }
    
    if(this->nos[solucao[i]].peso == 0){
      contador_dias++;
      pesos_arestas.clear();
    }

  }
  std::set<int> numeros;
  for (int numero : solucao) {
    numeros.insert(numero);
  }
  //std::cout << numeros.size() << " teste "<<  solucao.size() << std::endl;
   if(numeros.size() != solucao.size()){
    return false;
   }
   //std::cout << "numeros-> "<<numeros.size() << "solucao-> " << solucao.size() << std::endl;
  return true;

}

int Grafo::calculaCusto(std::vector<int> solucao){
  
  int valor_solucao = 0;

  for(auto& valor : solucao){
    valor_solucao += this->nos[valor].peso;
  }

  return valor_solucao;

}

int Grafo::quantidadeNos(){
  return nos.size();

}

