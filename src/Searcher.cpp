#include "Searcher.h"
#include <random>
#include <chrono>

using namespace std;

const Metrics& Searcher::getMetrics() const { return metrics_; }
Metrics        Searcher::copyMetrics()  const { return metrics_; }

int Searcher::buscaSequencial(vector<int>& vetor, int valorBusca){
    metrics_.reset();
    metrics_.auxMemBytes        = sizeof(int) + sizeof(bool) + 2 * sizeof(int);
    metrics_.maxRecursionDepth  = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int position = 0;
    for (int v : vetor)
    {
        countComparison();
        if (v == valorBusca)
        {
            return position;
            break;
        }
        position++;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    
    return -1;
}

int Searcher::buscaBinaria(vector<int>& vetor, int valorBusca){
    metrics_.reset();
    metrics_.auxMemBytes        = sizeof(int) + sizeof(bool) + 2 * sizeof(int);
    metrics_.maxRecursionDepth  = 0;

    auto t0 = std::chrono::high_resolution_clock::now();    

    int left = 0, right = vetor.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (vetor[mid] == valorBusca) {
            countComparison();
            return mid;
            break;
        }
        if (vetor[mid] < valorBusca) {
            countComparison();
            left = mid + 1;
        } else {
            countComparison();
            right = mid - 1;
        }
    }
    
    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    
    return -1;
}
