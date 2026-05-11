#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "DataGenerator.h"
#include "ExperimentRunner.h"
#include "Sorter.h"
#include "Searcher.h"

struct AlgoritmoOrdenacaoConfig {
    std::string nome;
    std::function<void(Sorter&, std::vector<int>&)> func;
};

struct AlgoritmoBuscaConfig {
    std::string nome;
    ExperimentRunner::SearchFunction func;
};

struct CenarioOrdenacao {
    std::string nome;
    std::function<std::vector<int>(int)> gerador;
};

struct CenarioBusca {
    std::string nome;
    std::function<int(const std::vector<int>&)> alvo;
};

// Funções para obter as listas
std::vector<AlgoritmoOrdenacaoConfig> getAlgoritmosOrdenacao() {
    return {
        {"Bubble Sort",     [](Sorter& s, std::vector<int>& v) { s.bubbleSort(v); }},
        {"Insertion Sort",  [](Sorter& s, std::vector<int>& v) { s.insertionSort(v); }},
        {"Selection Sort",  [](Sorter& s, std::vector<int>& v) { s.selectionSort(v); }},
        {"Merge Sort",      [](Sorter& s, std::vector<int>& v) { s.mergeSort(v); }},
        {"Heap Sort",       [](Sorter& s, std::vector<int>& v) { s.heapSort(v); }},
        {"Shell Sort",      [](Sorter& s, std::vector<int>& v) { s.shellSort(v); }},
        {"Quick Sort",      [](Sorter& s, std::vector<int>& v) { s.quickSort(v); }},
        {"Quick Random",    [](Sorter& s, std::vector<int>& v) { s.quickSortRandom(v); }}
    };
}

std::vector<CenarioOrdenacao> getCenariosOrdenacao() {
    return {
        {"Aleatorio",       [](int t) { return GeradorDeVetor::gerarValoresAleatorios(t); }},
        {"Ordenado",        [](int t) { return GeradorDeVetor::gerarValoresOrdenados(t, false); }},
        {"Reverso",         [](int t) { return GeradorDeVetor::gerarValoresOrdenados(t, true); }},
        {"Parcial",         [](int t) { return GeradorDeVetor::gerarValoresParcialmenteOrdenados(t); }},
        {"Repetitivo",      [](int t) { return GeradorDeVetor::gerarValoresAltamenteRepetidos(t); }},
        {"Gaussiano",       [](int t) { return GeradorDeVetor::gerarValoresComDistribuicaoGaussiana(t, t/2.0, t/10.0); }}
    };
}

std::vector<AlgoritmoBuscaConfig> getAlgoritmosBusca() {
    return {
        {"Busca Sequencial", [](Searcher& s, std::vector<int>& v, int alvo) { return s.buscaSequencial(v, alvo); }},
        {"Busca Binaria",    [](Searcher& s, std::vector<int>& v, int alvo) { return s.buscaBinaria(v, alvo); }}
    };
}

std::vector<CenarioBusca> getCenariosBusca() {
    return {
        {"Inicio",      [](const std::vector<int>& v) { return v.front(); }},
        {"Meio",        [](const std::vector<int>& v) { return v[v.size() / 2]; }},
        {"Fim",         [](const std::vector<int>& v) { return v.back(); }},
        {"Aleatorio",   [](const std::vector<int>& v) { return v[rand() % v.size()]; }},
        {"Inexistente", [](const std::vector<int>&)   { return -1; }}
    };
}

int main() {
    auto algoritmosSort = getAlgoritmosOrdenacao();
    auto cenariosSort   = getCenariosOrdenacao();
    auto algoritmosBusca = getAlgoritmosBusca();
    auto cenariosBusca   = getCenariosBusca();

    std::cout << "\n=========================================\n";
    std::cout << "INICIANDO EXPERIMENTOS DE ORDENACAO\n";
    std::cout << "=========================================\n";

    //Definição dos Tamanhos
    //std::vector<int> tamanhos = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    std::vector<int> tamanhos = {1000, 5000, 10000}; //Versão para testes

    //LOOP DE ORDENAÇÃO
    for (int tamanho : tamanhos) {
        for (const auto& cenario : cenariosSort) {
            for (const auto& algo : algoritmosSort) {
                
                // Executa 30 vezes e tira a média
                Metrics media = ExperimentRunner::runSortExperiment(
                    algo.nome,
                    cenario.nome,
                    algo.func,
                    cenario.gerador, 
                    tamanho, 
                    30
                );

                // Salva no CSV
                //ExperimentRunner::saveMetricsToCSV(arquivoSaida, algo.nome, cenario.nome, tamanho, media);
            }
        }
    }

    std::cout << "\n=========================================\n";
    std::cout << "INICIANDO EXPERIMENTOS DE BUSCA\n";
    std::cout << "=========================================\n";

    // --- LOOP DE BUSCA ---
    for (int tamanho : tamanhos) {
        std::vector<int> vetorBusca = GeradorDeVetor::gerarValoresOrdenados(tamanho, false);

        // Loop de execução
        for (const auto& cenario : cenariosBusca) {
            int alvo = cenario.alvo(vetorBusca);
            for (const auto& algoritmo : algoritmosBusca) {
                ExperimentRunner::runSearchExperiment(
                    algoritmo.nome, 
                    cenario.nome, 
                    algoritmo.func, 
                    vetorBusca, 
                    alvo, 
                    30
                );
            }
        }
    }

    std::cout << "\n>>> Todos os experimentos foram concluidos e salvos nos arquivos CSVs <<<\n";

    return 0;
}