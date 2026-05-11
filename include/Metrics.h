#pragma once

#include <cstdint>
#include <string>

/**
 * Metrics
 * =======
 * Estrutura que registra todos os dados de desempenho de UMA execução
 * de um algoritmo. Passada por referência para cada algoritmo do Sorter.
 */
struct Metrics {
    
    // Contadores de operações    

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

    
    // Recursão e memória
    
    //Profundidade máxima atingida na pilha de recursão.
    int maxRecursionDepth = 0;

    //Memória auxiliar alocada pelo algoritmo, em bytes.
    long long auxMemBytes = 0;


    // Tempo de execução

    /** Tempo de execução em nanossegundos (medido via std::chrono) */
    long long timeNs = 0;

    
    // Utilitários
    
    /** Zera todos os contadores. Chamado pelo Sorter antes de cada execução. */
    void reset();

    /** Imprime um resumo formatado para debug/relatório. */
    void print(const std::string& label = "") const;
};