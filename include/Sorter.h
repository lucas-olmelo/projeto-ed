#pragma once

#include "Metrics.h"
#include <vector>

/**
 * Sorter
 * ======
 * Módulo responsável pela implementação e instrumentação dos algoritmos
 * de ordenação. Cada método ordena o vetor IN-PLACE e registra todas as
 * métricas exigidas pela banca na struct interna `metrics`.
 *
 * DESIGN — por que Metrics é membro da classe?
 * ---------------------------------------------
 * Centraliza o acesso: o ExperimentRunner chama sort(), depois lê
 * sorter.getMetrics() — sem precisar gerenciar objetos Metrics externos.
 * O reset() é chamado automaticamente no início de cada sort, garantindo
 * que não haja contaminação entre execuções consecutivas.
 *
 * COMPATIBILIDADE COM O DataGenerator DO PARCEIRO
 * -----------------------------------------------
 * Os métodos recebem std::vector<int>& independente de como o vetor foi
 * gerado (LCG manual ou std::mt19937). O Sorter não tem acoplamento com
 * o DataGenerator — qualquer vetor de inteiros é aceito.
 *
 * USO TÍPICO (ExperimentRunner):
 *   Sorter sorter;
 *   for (int run = 0; run < 30; run++) {
 *       auto v = generator.uniform(n);   // cópia nova a cada rodada
 *       sorter.bubbleSort(v);
 *       results.push_back(sorter.getMetrics());
 *   }
 */
class Sorter {
public:
    Sorter() = default;

    // =========================================================================
    // Algoritmos de ordenação — Parte 2 do projeto
    // Implementados neste arquivo: os três algoritmos O(n²)
    // =========================================================================

    /**
     * BUBBLE SORT
     * -----------
     * Estratégia: percorre o vetor repetidamente, trocando elementos adjacentes
     * fora de ordem. A cada passagem, o maior elemento "borbulha" para o fim.
     *
     * Otimização implementada: flag `swapped` — interrompe cedo se nenhuma
     * troca ocorreu na passagem (vetor já ordenado). Isso torna o melhor caso O(n).
     *
     * Complexidade:
     *   Pior caso  (reverso):   O(n²) comparações, O(n²) trocas
     *   Caso médio (aleatório): O(n²) comparações, O(n²) trocas
     *   Melhor caso (ordenado): O(n)  comparações, O(1)  trocas
     *   Espaço: O(1) auxiliar — in-place
     *   Estável: SIM
     *
     * @param v  Vetor a ser ordenado (modificado in-place)
     */
    void bubbleSort(std::vector<int>& v);

    /**
     * INSERTION SORT
     * --------------
     * Estratégia: mantém um prefixo ordenado e insere cada novo elemento
     * na posição correta dentro desse prefixo, deslocando os maiores para a direita.
     *
     * Observação importante para o relatório: Insertion Sort é ADAPTATIVO —
     * seu custo real depende do número de inversões no vetor. Em dados
     * parcialmente ordenados, pode superar algoritmos O(n log n) para n pequeno.
     *
     * Complexidade:
     *   Pior caso  (reverso):           O(n²) comparações, O(n²) movimentações
     *   Caso médio (aleatório):         O(n²) comparações
     *   Melhor caso (ordenado):         O(n)  comparações, O(1)  movimentações
     *   Espaço: O(1) auxiliar — in-place
     *   Estável: SIM
     *
     * @param v  Vetor a ser ordenado (modificado in-place)
     */
    void insertionSort(std::vector<int>& v);

    /**
     * SELECTION SORT
     * --------------
     * Estratégia: a cada iteração, encontra o menor elemento no sufixo não
     * ordenado e o coloca na posição correta com uma única troca.
     *
     * Característica marcante: faz SEMPRE exatamente n*(n-1)/2 comparações,
     * independente do cenário. O número de trocas é no máximo n-1 (excelente
     * para situações onde trocas são caras, ex: disco).
     * NÃO é adaptativo — ordenado ou reverso, sempre O(n²).
     *
     * Complexidade:
     *   Todos os casos: O(n²) comparações, O(n) trocas
     *   Espaço: O(1) auxiliar — in-place
     *   Estável: NÃO (a troca pode alterar a ordem relativa de iguais)
     *
     * @param v  Vetor a ser ordenado (modificado in-place)
     */
    void selectionSort(std::vector<int>& v);

    // =========================================================================
    // Acesso às métricas
    // =========================================================================

    /**
     * Retorna as métricas da última execução.
     * Deve ser chamado APÓS um dos métodos de sort.
     */
    const Metrics& getMetrics() const;

    /**
     * Retorna uma cópia das métricas (para armazenar em containers).
     * Útil no ExperimentRunner ao coletar as 30 execuções.
     */
    Metrics copyMetrics() const;

private:
    Metrics metrics_;   ///< Estado interno de instrumentação

    // -------------------------------------------------------------------------
    // Helpers de instrumentação — encapsulam os incrementos para não poluir
    // a lógica dos algoritmos com código de contagem repetitivo.
    // -------------------------------------------------------------------------

    /**
     * Registra uma comparação entre dois elementos.
     * Chame ANTES de avaliar a condição (ex: if (v[i] > v[j])).
     * Incrementa comparisons +1 e memAccesses +2 (leu v[i] e v[j]).
     */
    inline void countComparison(int /*a*/, int /*b*/) {
        metrics_.comparisons++;
        metrics_.memAccesses += 2;  // leu dois elementos para comparar
    }

    /**
     * Registra uma troca (swap) entre duas posições.
     * Chame APÓS realizar o std::swap.
     * Incrementa swaps +1 e memAccesses +4 (leu e escreveu os dois elementos).
     */
    inline void countSwap() {
        metrics_.swaps++;
        metrics_.memAccesses += 4;  // 2 leituras + 2 escritas
    }

    /**
     * Registra uma movimentação simples (atribuição a uma posição do vetor).
     * Usado no Insertion Sort onde deslocamos elementos (não trocamos).
     * Incrementa swaps +1 (contado como movimentação) e memAccesses +2.
     */
    inline void countMove() {
        metrics_.swaps++;           // banca pede "trocas/movimentações" no mesmo contador
        metrics_.memAccesses += 2;  // 1 leitura + 1 escrita
    }
};