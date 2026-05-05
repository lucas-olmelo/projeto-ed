#include "Sorter.h"

#include <chrono>    // std::chrono::high_resolution_clock
#include <algorithm> // std::swap

// =============================================================================
// Acesso às métricas
// =============================================================================

const Metrics& Sorter::getMetrics() const {
    return metrics_;
}

Metrics Sorter::copyMetrics() const {
    return metrics_;
}

// =============================================================================
// BUBBLE SORT
// =============================================================================

void Sorter::bubbleSort(std::vector<int>& v) {
    metrics_.reset();

    // Memória auxiliar: apenas variáveis escalares — O(1) bytes
    metrics_.auxMemBytes = sizeof(int)  // temp do swap
                         + sizeof(bool) // flag swapped
                         + sizeof(int)  // índice i
                         + sizeof(int); // índice j

    // Profundidade de recursão: 0 (algoritmo iterativo)
    metrics_.maxRecursionDepth = 0;

    // ----- início da cronometragem -----
    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());

    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;

        // A cada passagem i, os últimos i elementos já estão no lugar certo.
        // Só precisamos verificar até (n - 1 - i).
        for (int j = 0; j < n - 1 - i; ++j) {

            // Registra a comparação antes de avaliar
            countComparison(v[j], v[j + 1]);

            if (v[j] > v[j + 1]) {
                std::swap(v[j], v[j + 1]);
                countSwap();
                swapped = true;
            }
        }

        // Otimização: se nenhuma troca ocorreu, o vetor já está ordenado.
        // Transforma o melhor caso em O(n) — importante para dados ordenados.
        if (!swapped) break;
    }

    // ----- fim da cronometragem -----
    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// INSERTION SORT
// =============================================================================

void Sorter::insertionSort(std::vector<int>& v) {
    metrics_.reset();

    // Memória auxiliar: variável `key` para guardar o elemento sendo inserido
    // mais os índices de controle do loop — O(1) bytes
    metrics_.auxMemBytes = sizeof(int)  // key
                         + sizeof(int)  // índice i
                         + sizeof(int); // índice j

    metrics_.maxRecursionDepth = 0;

    // ----- início da cronometragem -----
    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());

    // Elemento 0 já está "ordenado" — começa no índice 1
    for (int i = 1; i < n; ++i) {

        int key = v[i];          // salva o elemento a ser inserido
        metrics_.memAccesses++;  // leitura de v[i] para key

        int j = i - 1;

        // Desloca para a direita todos os elementos maiores que key.
        // Cada iteração desse while é uma MOVIMENTAÇÃO (não uma troca completa).
        while (j >= 0) {

            // Registra a comparação com o elemento à esquerda
            countComparison(v[j], key);

            if (v[j] > key) {
                v[j + 1] = v[j]; // desloca v[j] uma posição para a direita
                countMove();
                j--;
            } else {
                break; // posição correta encontrada
            }
        }

        // Insere key na posição correta (j+1)
        v[j + 1] = key;
        metrics_.memAccesses++;  // escrita na posição final

        // Só conta como movimentação se houve deslocamento real
        if (j != i - 1) {
            metrics_.swaps++;    // a inserção final também é uma movimentação
        }
    }

    // ----- fim da cronometragem -----
    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// SELECTION SORT
// =============================================================================

void Sorter::selectionSort(std::vector<int>& v) {
    metrics_.reset();

    // Memória auxiliar: índice do mínimo + índices de controle — O(1) bytes
    metrics_.auxMemBytes = sizeof(int)  // minIdx
                         + sizeof(int)  // índice i
                         + sizeof(int); // índice j

    metrics_.maxRecursionDepth = 0;

    // ----- início da cronometragem -----
    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());

    for (int i = 0; i < n - 1; ++i) {

        // Encontra o índice do menor elemento no sufixo v[i..n-1]
        int minIdx = i;

        for (int j = i + 1; j < n; ++j) {

            // Registra comparação com o atual mínimo candidato
            countComparison(v[j], v[minIdx]);

            if (v[j] < v[minIdx]) {
                minIdx = j;
                // Nota: atualizar minIdx não é acesso ao vetor de dados —
                // é apenas atualização de índice, não contamos aqui.
            }
        }

        // Só troca se o mínimo não estiver já na posição correta.
        // Selection Sort faz no máximo n-1 trocas — daí sua vantagem
        // em cenários onde trocas são custosas.
        if (minIdx != i) {
            std::swap(v[i], v[minIdx]);
            countSwap();
        }
    }

    // ----- fim da cronometragem -----
    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}