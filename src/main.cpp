#include <iostream>
#include <string>
#include "DataGenerator.h"
#include "Sorter.h"
#include "Searcher.h"
#include <algorithm>

using namespace std;

void imprimir(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
}

int main() {
    // 1. Criar um vetor de exemplo (pode vir de um gerador ou entrada manual)
    std::vector<int> meuVetor = GeradorDeVetor::gerarValoresAleatorios(10);
 
    std::cout << "Vetor original: ";
    for (int x : meuVetor) std::cout << x << " ";
    std::cout << "\n-----------------------------------\n";
 
    // 2. Instanciar o Sorter (a seed 42 é o padrão no .h)
    Sorter ordenador(42);
 
    // 3. Aplicar o Bubble Sort
    // O vetor é passado por referência e será alterado in-place
    ordenador.bubbleSort(meuVetor);
 
    // 4. Acessar as métricas coletadas durante a execução
    Metrics m = ordenador.getMetrics();
 
    // 5. Exibir resultados
    std::cout << "Vetor ordenado: ";
    for (int x : meuVetor) std::cout << x << " ";
    std::cout << "\n\n--- Metricas do Bubble Sort ---" << std::endl;
    std::cout << "Comparacoes: " << m.comparisons << std::endl;
    std::cout << "Trocas (Swaps): " << m.swaps << std::endl;
    std::cout << "Acessos a Memoria: " << m.memAccesses << std::endl;
    std::cout << "Tempo (ns): " << m.timeNs << " ns" << std::endl;
    std::cout << "Memoria Auxiliar (bytes): " << m.auxMemBytes << std::endl;

    Searcher searcher;
    searcher.buscaSequencial(meuVetor, 45);
 
    return 0;
}