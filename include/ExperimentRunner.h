#ifndef EXPERIMENTRUNNER_H
#define EXPERIMENTRUNNER_H

#include <string>
#include <vector>
#include <functional>
#include "Metrics.h"
#include "Sorter.h"
#include "Searcher.h"

class ExperimentRunner {
public:
    // Define assinaturas para facilitar a leitura
    using SortFunction      = std::function<void(Sorter&, std::vector<int>&)>;
    using GeneratorFunction = std::function<std::vector<int>(int)>;
    using SearchFunction    = std::function<int(Searcher&, std::vector<int>&, int)>;

    //Executa um algoritmo de ordenação N vezes e retorna a média das métricas.
    static Metrics runSortExperiment(
        const std::string& nomeAlgoritmo,
        const std::string& nomeCenario,
        SortFunction algoritmo,
        GeneratorFunction gerador,
        int tamanho,
        int numExecucoes = 30
    );

    static Metrics runSearchExperiment(
        const std::string& nomeAlgoritmo,
        const std::string& nomeCenario,
        SearchFunction algoritmo,
        std::vector<int>& vetorBase,
        int valorBusca,
        int numExecucoes = 30
    );

    //Salva as métricas (médias ou únicas) em um arquivo CSV.
    static void saveMetricsToCSV(
        const std::string& nomeArquivo, 
        const std::string& algoritmo, 
        const std::string& distribuicao,
        int tamanho,
        const Metrics& m,
        int execucaoId = -1
    );
};

#endif