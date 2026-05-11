#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "DataGenerator.h"
#include "ExperimentRunner.h"
#include "Sorter.h"
#include "Searcher.h"

#include <map>
#include <string>
#include <functional>
#include <iostream>

// No seu main.cpp ou em um cabeçalho de config
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
        {"Quick Sort",      [](Sorter& s, std::vector<int>& v) { s.quickSort(v); }},
        {"Quick Random",    [](Sorter& s, std::vector<int>& v) { s.quickSortRandom(v); }},
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

/*// Definimos um tipo para facilitar a leitura: um ponteiro para método da classe Sorter
typedef void (Sorter::*SortMethod)(std::vector<int>&);
typedef void (Searcher::*SearchMethod)(std::vector<int>&, int);

void executarOrdenacao(Sorter& ordenador, std::vector<int>& meuVetor, const std::string& nomeAlgoritmo) {
    static std::map<std::string, SortMethod> dicionarioSorters = {
        {"Bubble sort",         &Sorter::bubbleSort},
        {"Selection sort",      &Sorter::selectionSort},
        {"Insertion sort",      &Sorter::insertionSort},
        {"Merge sort",          &Sorter::mergeSort},
        {"Heap sort",           &Sorter::heapSort},
        {"Shell sort",          &Sorter::shellSort},
        {"Quick sort",          &Sorter::quickSort},
        {"Quick sort Random",   &Sorter::quickSortRandom}
    };

    if (dicionarioSorters.count(nomeAlgoritmo)) {
        SortMethod metodo = dicionarioSorters[nomeAlgoritmo];

        (ordenador.*metodo)(meuVetor);
        std::cout << "Executado: " << nomeAlgoritmo << std::endl;
    } else {
        std::cerr << "Erro: Algoritmo '" << nomeAlgoritmo << "' nao encontrado!" << std::endl;
    }
}

void executarBusca(Searcher& searcher, std::vector<int>& meuVetor, int valorBusca, const std::string& nomeAlgoritmo) {
    static std::map<std::string, SearchMethod> dicionarioSorters = {
        {"Busca Sequencial",   &Searcher::buscaSequencial},
        {"Busca Binaria",   &Searcher::buscaBinaria}
    };

    if (dicionarioSorters.count(nomeAlgoritmo)) {
        SearchMethod metodo = dicionarioSorters[nomeAlgoritmo];

        (searcher.*metodo)(meuVetor, valorBusca);
        std::cout << "Executado: " << nomeAlgoritmo << std::endl;
    } else {
        std::cerr << "Erro: Algoritmo '" << nomeAlgoritmo << "' nao encontrado!" << std::endl;
    }
}

void salvarMetricasCSV(const std::string& nomeArquivo, const std::string& algoritmo, const Metrics& m) {
    std::ofstream arquivo;
    
    std::ifstream checarArquivo(nomeArquivo);
    bool novoArquivo = !checarArquivo.is_open();
    checarArquivo.close();

    arquivo.open(nomeArquivo, std::ios::app);

    if (arquivo.is_open()) {
        if (novoArquivo) {
            // CORREÇÃO LINHA 24: Removido o caminho do Windows que causava erro
            arquivo << "Algoritmo,Comparacoes,Trocas,AcessosMemoria,TempoNS,MemoriaAuxiliarBytes\n";
        }

        arquivo << algoritmo << ","
                << m.comparisons << ","
                << m.swaps << ","
                << m.memAccesses << ","
                << m.timeNs << ","
                << m.auxMemBytes << "\n";

        arquivo.close();
        std::cout << "Metricas exportadas com sucesso para " << nomeArquivo << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para exportacao!" << std::endl;
    }
}*/

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
    //Versão para testes
    std::vector<int> tamanhos = {1000};

    std::string arquivoSaida = "resultados_finais.csv";

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
                ExperimentRunner::saveMetricsToCSV(arquivoSaida, algo.nome, cenario.nome, tamanho, media);
            }
        }
    }

    std::cout << "\n>>> Todos os experimentos foram concluidos e salvos em " << arquivoSaida << " <<<\n";

    std::cout << "\n=========================================\n";
    std::cout << "INICIANDO EXPERIMENTOS DE BUSCA\n";
    std::cout << "=========================================\n";

    // --- LOOP DE BUSCA ---
    for (int tamanho : tamanhos) {
        // Para buscas justas, o vetor deve estar ordenado e sem repeticoes no pior caso.
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

    return 0;
    // // 1. Gerar vetor
    // std::vector<int> meuVetor = GeradorDeVetor::gerarValoresAleatorios(1000);
 
    // std::cout << "Vetor original: ";
    // for (int x : meuVetor) std::cout << x << " ";
    // std::cout << "\n-----------------------------------\n";
 
    // // 2. Instanciar ordenador
    // Sorter ordenador(42);
 
    // // 3. Bubble Sort
    // ordenador.bubbleSort(meuVetor);
 
    // // 4. Coletar métricas
    // Metrics m = ordenador.getMetrics();
 
    // // 5. Exibir resultados
    // std::cout << "Vetor ordenado: ";
    // for (int x : meuVetor) std::cout << x << " ";
    // std::cout << "\n\n--- Metricas do Bubble Sort ---\n";
    // std::cout << "Comparacoes: " << m.comparisons << "\n";
    // std::cout << "Trocas: " << m.swaps << "\n";
    // std::cout << "Tempo (ns): " << m.timeNs << " ns\n";

    // // 6. Busca (Exemplo)
    // Searcher searcher;
    // searcher.buscaSequencial(meuVetor, 6954);

    // Metrics met = searcher.getMetrics();
    // std::cout << "\n\n--- Metricas da Busca Sequencial ---\n";
    // std::cout << "Comparacoes: " << met.comparisons << "\n";
    // std::cout << "Acesso Memoria: " << met.memAccesses << "\n";
    // std::cout << "Recursão: " << met.maxRecursionDepth << "\n";
    // std::cout << "Tempo (ns): " << met.timeNs << " ns\n";

    // searcher.buscaBinaria(meuVetor, 6954);

    // Metrics met2 = searcher.getMetrics();
    // std::cout << "\n\n--- Metricas da Busca Binaria ---\n";
    // std::cout << "Comparacoes: " << met2.comparisons << "\n";
    // std::cout << "Acesso Memoria: " << met2.memAccesses << "\n";
    // std::cout << "Recursão: " << met2.maxRecursionDepth << "\n";
    // std::cout << "Tempo (ns): " << met2.timeNs << " ns\n";


    // // 7. Exportar para CSV
    // salvarMetricasCSV("src/output/resultados_ordenacao_BubbleSort.csv", "BubbleSort", m);


    // // 8. Utilizando função para varios algoritmos

    // executarOrdenacao(ordenador, meuVetor, "Insertion sort");
    // m = ordenador.getMetrics();
    // salvarMetricasCSV("src/output/resultados_ordenacao_InsertionSort.csv", "InsertionSort", m);

    // executarBusca(searcher, meuVetor, 9901, "Busca Binaria");
    // met = searcher.getMetrics();
    // salvarMetricasCSV("src/output/resultados_busca_binaria.csv", "BuscaBinaria", met);

    // executarBusca(searcher, meuVetor, 9901, "Busca Sequencial");
    // met2 = searcher.getMetrics();
    // salvarMetricasCSV("src/output/resultados_busca_sequencial.csv", "BuscaSequencial", met2);
    
    
    // return 0;
}