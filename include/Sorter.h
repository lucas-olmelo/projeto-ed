#pragma once

#include "Metrics.h"
#include <vector>
#include <cstdint>  // uint64_t (LCG do Quick Sort randomizado)

/**
 * Sorter
 * ======
 * Implementa e instrumenta os 8 algoritmos de ordenação obrigatórios.
 * Cada método ordena o vetor IN-PLACE e registra métricas completas
 * na struct interna `metrics_`.
 *
 * ALGORITMOS IMPLEMENTADOS
 * ------------------------
 *  O(n²)      — Bubble Sort, Insertion Sort, Selection Sort
 *  O(n log n) — Merge Sort, Heap Sort, Shell Sort*
 *  O(n log n) — Quick Sort (pivô fixo), Quick Sort randomizado
 *  (* Shell Sort: complexidade depende da sequência de gaps)
 *
 * DESIGN — Metrics como membro
 * -----------------------------
 * O reset() é chamado automaticamente no início de cada sort.
 * O ExperimentRunner chama sort() → copyMetrics() em loop de 30x,
 * sem precisar gerenciar objetos Metrics externos.
 *
 * QUICK SORT RANDOMIZADO — reprodutibilidade
 * -------------------------------------------
 * Usa LCG interno com seed configurável (padrão: 42), garantindo
 * que a escolha do pivô seja reproduzível — exigência da banca.
 * Compatível com qualquer DataGenerator (LCG manual ou mt19937).
 */
class Sorter {
public:
    /**
     * @param lcgSeed  Seed do LCG usado pelo Quick Sort randomizado.
     *                 Padrão 42 garante reprodutibilidade entre execuções.
     */
    explicit Sorter(uint64_t lcgSeed = 42);

    // =========================================================================
    // Algoritmos O(n²)
    // =========================================================================

    /**
     * BUBBLE SORT
     * Percorre adjacências repetidamente trocando pares fora de ordem.
     * Otimização: flag swapped → melhor caso O(n) para vetor já ordenado.
     *
     * Complexidade:  pior/médio O(n²) | melhor O(n)
     * Estável: SIM   |   In-place: SIM   |   Recursivo: NÃO
     */
    void bubbleSort(std::vector<int>& v);

    /**
     * INSERTION SORT
     * Mantém prefixo ordenado; insere cada elemento na posição correta
     * deslocando os maiores para a direita. Adaptativo ao nº de inversões.
     *
     * Complexidade:  pior/médio O(n²) | melhor O(n)
     * Estável: SIM   |   In-place: SIM   |   Recursivo: NÃO
     */
    void insertionSort(std::vector<int>& v);

    /**
     * SELECTION SORT
     * Seleciona o mínimo do sufixo e o posiciona com uma troca.
     * Sempre exatamente n*(n-1)/2 comparações — não-adaptativo.
     *
     * Complexidade:  todos os casos O(n²), máximo n-1 trocas
     * Estável: NÃO   |   In-place: SIM   |   Recursivo: NÃO
     */
    void selectionSort(std::vector<int>& v);

    // =========================================================================
    // Algoritmos O(n log n)
    // =========================================================================

    /**
     * MERGE SORT
     * Divide o vetor ao meio recursivamente e mescla as metades ordenadas.
     * Único algoritmo aqui que requer memória auxiliar O(n) — vetor temporário
     * alocado na fusão.
     *
     * Complexidade:  todos os casos Θ(n log n)
     * Estável: SIM   |   In-place: NÃO (O(n) aux)   |   Recursivo: SIM
     * maxRecursionDepth ≈ ⌊log₂ n⌋
     */
    void mergeSort(std::vector<int>& v);

    /**
     * HEAP SORT
     * Constrói um max-heap e extrai o máximo repetidamente para o fim.
     * A fase de construção do heap usa heapify iterativo (bottom-up).
     *
     * Complexidade:  todos os casos Θ(n log n)
     * Estável: NÃO   |   In-place: SIM   |   Recursivo: NÃO (heapify iterativo)
     * maxRecursionDepth = 0
     */
    void heapSort(std::vector<int>& v);

    /**
     * SHELL SORT
     * Generalização do Insertion Sort com gaps decrescentes (sequência de Knuth:
     * 1, 4, 13, 40, 121, ...). Reduz inversões distantes antes do passo final.
     *
     * Complexidade:  O(n^(4/3)) com sequência de Knuth (melhor que O(n²))
     * Estável: NÃO   |   In-place: SIM   |   Recursivo: NÃO
     */
    void shellSort(std::vector<int>& v);

    /**
     * QUICK SORT (pivô fixo — último elemento)
     * Particionamento de Lomuto: pivô = v[high], particiona em ≤pivô e >pivô.
     * Pior caso O(n²) em dados já ordenados ou com pivô ruim.
     *
     * Complexidade:  pior O(n²) | médio/melhor Θ(n log n)
     * Estável: NÃO   |   In-place: SIM   |   Recursivo: SIM
     * maxRecursionDepth ≈ log₂ n (médio) ou n (pior caso)
     */
    void quickSort(std::vector<int>& v);

    /**
     * QUICK SORT RANDOMIZADO
     * Idêntico ao Quick Sort simples, exceto que o pivô é escolhido
     * aleatoriamente via LCG interno com seed fixa — garante reprodutibilidade.
     * Reduz drasticamente a probabilidade do pior caso O(n²).
     *
     * Complexidade:  pior O(n²) com probabilidade desprezível | médio Θ(n log n)
     * Estável: NÃO   |   In-place: SIM   |   Recursivo: SIM
     */
    void quickSortRandom(std::vector<int>& v);

    // =========================================================================
    // Acesso às métricas
    // =========================================================================

    /** Retorna referência às métricas da última execução. */
    const Metrics& getMetrics() const;

    /** Retorna cópia das métricas — use no ExperimentRunner para guardar resultados. */
    Metrics copyMetrics() const;

    /** Redefine a seed do LCG usado pelo Quick Sort randomizado. */
    void setLcgSeed(uint64_t seed);

private:
    Metrics  metrics_;       ///< Instrumentação da execução atual
    uint64_t lcgState_;      ///< Estado do LCG (Quick Sort randomizado)
    uint64_t lcgSeedOrig_;   ///< Seed original — para reset entre execuções

    // -------------------------------------------------------------------------
    // LCG interno (Knuth MMIX) — usado pelo Quick Sort randomizado
    // -------------------------------------------------------------------------
    uint64_t lcgNext();
    int      lcgRange(int lo, int hi);   ///< Inteiro aleatório em [lo, hi]
    void     lcgReset();                 ///< Restaura estado para lcgSeedOrig_

    // -------------------------------------------------------------------------
    // Helpers de instrumentação
    // -------------------------------------------------------------------------
    inline void countComparison() {
        metrics_.comparisons++;
        metrics_.memAccesses += 2;  // leu dois elementos
    }
    inline void countSwap() {
        metrics_.swaps++;
        metrics_.memAccesses += 4;  // 2 leituras + 2 escritas
    }
    inline void countMove() {
        metrics_.swaps++;
        metrics_.memAccesses += 2;  // 1 leitura + 1 escrita
    }

    // -------------------------------------------------------------------------
    // Auxiliares recursivos (recebem profundidade atual para rastrear máximo)
    // -------------------------------------------------------------------------
    void mergeSortHelper(std::vector<int>& v, int lo, int hi, int depth);
    void merge(std::vector<int>& v, int lo, int mid, int hi);

    void quickSortHelper(std::vector<int>& v, int lo, int hi, int depth);
    void quickSortRandomHelper(std::vector<int>& v, int lo, int hi, int depth);
    int  partitionLomuto(std::vector<int>& v, int lo, int hi);
    int  partitionLomutoRandom(std::vector<int>& v, int lo, int hi);

    void heapify(std::vector<int>& v, int n, int i);
};