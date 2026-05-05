#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * Metrics
 * =======
 * Estrutura que registra todos os dados de desempenho de UMA execução
 * de um algoritmo. Passada por referência para cada algoritmo do Sorter.
 *
 * Campos exigidos pela banca (Parte 3 do projeto):
 *   - comparações de chave
 *   - trocas / movimentações
 *   - acessos ao vetor (leitura + escrita)
 *   - profundidade máxima de recursão
 *   - tempo de execução em nanossegundos
 *   - memória auxiliar estimada em bytes
 */
struct Metrics {

    // -------------------------------------------------------------------------
    // Contadores de operações
    // -------------------------------------------------------------------------

    /** Número de comparações entre elementos (ex: v[i] < v[j]) */
    long long comparisons = 0;

    /** Número de trocas ou movimentações de elementos (ex: swap, atribuição) */
    long long swaps = 0;

    /**
     * Total de acessos ao vetor (leituras + escritas).
     * Cada leitura de v[i] conta +1, cada escrita em v[i] conta +1.
     * Permite medir pressão sobre o cache de forma mais fina que só comparações.
     */
    long long memAccesses = 0;

    // -------------------------------------------------------------------------
    // Recursão e memória
    // -------------------------------------------------------------------------

    /**
     * Profundidade máxima atingida na pilha de recursão.
     * 0 para algoritmos iterativos. Instrumentado manualmente via parâmetro
     * de profundidade em algoritmos como Merge Sort e Quick Sort.
     */
    int maxRecursionDepth = 0;

    /**
     * Memória auxiliar alocada pelo algoritmo, em bytes.
     * Exemplos:
     *   - Bubble/Insertion/Selection: 0 (in-place)
     *   - Merge Sort: sizeof(int) * n  (vetor auxiliar)
     *   - Quick Sort: sizeof(int) * profundidade  (pilha de recursão implícita)
     */
    long long auxMemBytes = 0;

    // -------------------------------------------------------------------------
    // Tempo de execução
    // -------------------------------------------------------------------------

    /** Tempo de execução em nanossegundos (medido via std::chrono) */
    long long timeNs = 0;

    // -------------------------------------------------------------------------
    // Utilitários
    // -------------------------------------------------------------------------

    /** Zera todos os contadores. Chamado pelo Sorter antes de cada execução. */
    void reset() {
        comparisons      = 0;
        swaps            = 0;
        memAccesses      = 0;
        maxRecursionDepth = 0;
        auxMemBytes      = 0;
        timeNs           = 0;
    }

    /** Imprime um resumo formatado para debug/relatório. */
    void print(const std::string& label = "") const {
        if (!label.empty())
            std::cout << "--- " << label << " ---\n";
        std::cout << std::left
                  << std::setw(22) << "  Comparacoes:"    << comparisons      << "\n"
                  << std::setw(22) << "  Trocas:"         << swaps            << "\n"
                  << std::setw(22) << "  Acessos mem.:"   << memAccesses      << "\n"
                  << std::setw(22) << "  Prof. recursao:" << maxRecursionDepth << "\n"
                  << std::setw(22) << "  Mem. auxiliar:"  << auxMemBytes << " bytes\n"
                  << std::setw(22) << "  Tempo:"          << timeNs      << " ns\n";
    }
};