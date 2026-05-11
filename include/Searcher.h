#ifndef Searcher_H
#define Searcher_H

#include "Metrics.h"
#include <vector>

class Searcher {
public:
    /** Retorna referência às métricas da última execução. */
    const Metrics& getMetrics() const;

    /** Retorna cópia das métricas — use no ExperimentRunner para guardar resultados. */
    Metrics copyMetrics() const;

    //Método 1: Busca Sequencial
    int buscaSequencial(std::vector<int>& vetor, int valorBusca);
    //Método 2: Busca Binária
    int buscaBinaria(std::vector<int>& vetor, int valorBusca);
private:
    Metrics metrics_;

    inline void countComparison() {
        metrics_.comparisons++;
        metrics_.memAccesses += 2;  // leu dois elementos
    }
};
#endif