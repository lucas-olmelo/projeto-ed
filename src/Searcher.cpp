#include "Searcher.h"
#include <random>
#include <chrono>

using namespace std;

const Metrics& Searcher::getMetrics() const { return metrics_; }
Metrics        Searcher::copyMetrics()  const { return metrics_; }

void Searcher::buscaSequencial(vector<int>& vetor, int valorBusca){
    metrics_.reset();
    metrics_.auxMemBytes        = sizeof(int) + sizeof(bool) + 2 * sizeof(int);
    metrics_.maxRecursionDepth  = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int position = 0;
    bool encontrar = false;
    for (int v : vetor)
    {
        countComparison();
        if (v == valorBusca)
        {
            std::cout << position;
            encontrar = true;
        }
        position++;
    }

    if (!encontrar) std::cout << "Não encontrou";
    
    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

void Searcher::buscaBinaria(vector<int>& vetor, int valorBusca){

}
