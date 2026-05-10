#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "DataGenerator.h"
#include "Sorter.h"
#include "Searcher.h"


#include <map>
#include <string>
#include <functional>
#include <iostream>

// Definimos um tipo para facilitar a leitura: um ponteiro para método da classe Sorter
typedef void (Sorter::*SortMethod)(std::vector<int>&);

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
}


// Main -> Exemplo de uso do Sorter e coleta de métricas
int main() {
    // 1. Gerar vetor
    std::vector<int> meuVetor = GeradorDeVetor::gerarValoresAleatorios(10);
 
    std::cout << "Vetor original: ";
    for (int x : meuVetor) std::cout << x << " ";
    std::cout << "\n-----------------------------------\n";
 
    // 2. Instanciar ordenador
    Sorter ordenador(42);
 
    // 3. Bubble Sort
    ordenador.bubbleSort(meuVetor);
 
    // 4. Coletar métricas
    Metrics m = ordenador.getMetrics();
 
    // 5. Exibir resultados
    std::cout << "Vetor ordenado: ";
    for (int x : meuVetor) std::cout << x << " ";
    std::cout << "\n\n--- Metricas do Bubble Sort ---\n";
    std::cout << "Comparacoes: " << m.comparisons << "\n";
    std::cout << "Trocas: " << m.swaps << "\n";
    std::cout << "Tempo (ns): " << m.timeNs << " ns\n";

    // 6. Busca (Exemplo)
    Searcher searcher;
    searcher.buscaSequencial(meuVetor, 45);

    // 7. Exportar para CSV
    salvarMetricasCSV("src/output/resultados_ordenacao_BubbleSort.csv", "BubbleSort", m);


    // 8. Utilizando função para varios algoritmos

    executarOrdenacao(ordenador, meuVetor, "Insertion sort");
    m = ordenador.getMetrics();
    salvarMetricasCSV("src/output/resultados_ordenacao_InsertionSort.csv", "InsertionSort", m);


    return 0;
}