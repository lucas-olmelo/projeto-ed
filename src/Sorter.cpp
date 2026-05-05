#include "Sorter.h"

#include <chrono>
#include <algorithm>  // std::swap
#include <vector>

// =============================================================================
// Constantes LCG (Knuth MMIX) — mesmo gerador do DataGenerator manual
// =============================================================================
static constexpr uint64_t LCG_A = 6364136223846793005ULL;
static constexpr uint64_t LCG_C = 1442695040888963407ULL;

// =============================================================================
// Construtor e LCG
// =============================================================================

Sorter::Sorter(uint64_t lcgSeed)
    : lcgState_(lcgSeed), lcgSeedOrig_(lcgSeed)
{
    // Aquece o LCG (afasta o estado do valor de seed de baixa entropia)
    for (int i = 0; i < 16; ++i) lcgNext();
}

void Sorter::setLcgSeed(uint64_t seed) {
    lcgSeedOrig_ = seed;
    lcgReset();
}

uint64_t Sorter::lcgNext() {
    lcgState_ = LCG_A * lcgState_ + LCG_C;
    return lcgState_;
}

int Sorter::lcgRange(int lo, int hi) {
    if (lo >= hi) return lo;
    uint64_t range     = static_cast<uint64_t>(hi - lo) + 1;
    uint64_t threshold = (UINT64_MAX - range + 1) % range;
    uint64_t r;
    do { r = lcgNext(); } while (r < threshold);
    return lo + static_cast<int>(r % range);
}

void Sorter::lcgReset() {
    lcgState_ = lcgSeedOrig_;
    for (int i = 0; i < 16; ++i) lcgNext();
}

// =============================================================================
// Acesso às métricas
// =============================================================================

const Metrics& Sorter::getMetrics() const { return metrics_; }
Metrics        Sorter::copyMetrics()  const { return metrics_; }

// =============================================================================
// BUBBLE SORT
// =============================================================================

void Sorter::bubbleSort(std::vector<int>& v) {
    metrics_.reset();
    metrics_.auxMemBytes      = sizeof(int) + sizeof(bool) + 2 * sizeof(int);
    metrics_.maxRecursionDepth = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            countComparison();
            if (v[j] > v[j + 1]) {
                std::swap(v[j], v[j + 1]);
                countSwap();
                swapped = true;
            }
        }
        if (!swapped) break;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// INSERTION SORT
// =============================================================================

void Sorter::insertionSort(std::vector<int>& v) {
    metrics_.reset();
    metrics_.auxMemBytes       = 3 * sizeof(int);
    metrics_.maxRecursionDepth = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; ++i) {
        int key = v[i];
        metrics_.memAccesses++;   // leitura de v[i]

        int j = i - 1;
        while (j >= 0) {
            countComparison();
            if (v[j] > key) {
                v[j + 1] = v[j];
                countMove();
                --j;
            } else {
                break;
            }
        }
        v[j + 1] = key;
        metrics_.memAccesses++;   // escrita na posição final
        if (j != i - 1) metrics_.swaps++;  // inserção final = 1 movimentação
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// SELECTION SORT
// =============================================================================

void Sorter::selectionSort(std::vector<int>& v) {
    metrics_.reset();
    metrics_.auxMemBytes       = 3 * sizeof(int);
    metrics_.maxRecursionDepth = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            countComparison();
            if (v[j] < v[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            std::swap(v[i], v[minIdx]);
            countSwap();
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// MERGE SORT
// =============================================================================

void Sorter::merge(std::vector<int>& v, int lo, int mid, int hi) {
    // Aloca vetor temporário para a fusão — isso é a memória auxiliar O(n)
    int leftSize  = mid - lo + 1;
    int rightSize = hi - mid;

    std::vector<int> L(v.begin() + lo,       v.begin() + mid + 1);
    std::vector<int> R(v.begin() + mid + 1,  v.begin() + hi  + 1);

    // Acumula a memória alocada nesta chamada
    metrics_.auxMemBytes += static_cast<long long>((leftSize + rightSize) * sizeof(int));

    int i = 0, j = 0, k = lo;
    while (i < leftSize && j < rightSize) {
        countComparison();
        if (L[i] <= R[j]) {
            v[k++] = L[i++];
        } else {
            v[k++] = R[j++];
        }
        metrics_.memAccesses++;  // escrita em v[k]
    }
    // Copia os elementos restantes
    while (i < leftSize)  { v[k++] = L[i++]; metrics_.memAccesses++; countMove(); }
    while (j < rightSize) { v[k++] = R[j++]; metrics_.memAccesses++; countMove(); }
}

void Sorter::mergeSortHelper(std::vector<int>& v, int lo, int hi, int depth) {
    // Atualiza a profundidade máxima de recursão atingida
    if (depth > metrics_.maxRecursionDepth)
        metrics_.maxRecursionDepth = depth;

    if (lo >= hi) return;

    int mid = lo + (hi - lo) / 2;   // evita overflow vs (lo+hi)/2

    mergeSortHelper(v, lo,    mid, depth + 1);
    mergeSortHelper(v, mid+1, hi,  depth + 1);
    merge(v, lo, mid, hi);
}

void Sorter::mergeSort(std::vector<int>& v) {
    metrics_.reset();
    // auxMemBytes será acumulado dentro de merge() a cada chamada.
    // Valor total esperado: O(n log n) alocações, mas no máximo O(n) simultâneas.
    metrics_.maxRecursionDepth = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    if (!v.empty())
        mergeSortHelper(v, 0, static_cast<int>(v.size()) - 1, 1);

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// HEAP SORT
// =============================================================================

/**
 * heapify iterativo (sift-down): mantém a propriedade de max-heap
 * para a subárvore com raiz em i, assumindo que os filhos já são heaps.
 * Iterativo evita overhead de recursão e mantém maxRecursionDepth = 0.
 */
void Sorter::heapify(std::vector<int>& v, int n, int i) {
    while (true) {
        int largest = i;
        int left    = 2 * i + 1;
        int right   = 2 * i + 2;

        // Compara com filho esquerdo
        if (left < n) {
            countComparison();
            if (v[left] > v[largest]) largest = left;
        }
        // Compara com filho direito
        if (right < n) {
            countComparison();
            if (v[right] > v[largest]) largest = right;
        }

        if (largest == i) break;  // heap já satisfeito

        std::swap(v[i], v[largest]);
        countSwap();
        i = largest;  // desce para o filho trocado
    }
}

void Sorter::heapSort(std::vector<int>& v) {
    metrics_.reset();
    metrics_.auxMemBytes       = 3 * sizeof(int);  // i, largest, left, right — O(1)
    metrics_.maxRecursionDepth = 0;                // algoritmo totalmente iterativo

    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());

    // FASE 1 — Construção do max-heap (bottom-up, O(n))
    // Começa no último nó interno: (n/2 - 1)
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(v, n, i);

    // FASE 2 — Extração: move a raiz (máximo) para o fim e reconstrói o heap
    for (int i = n - 1; i > 0; --i) {
        std::swap(v[0], v[i]);   // raiz (máximo) vai para a posição i
        countSwap();
        heapify(v, i, 0);        // reconstrói heap com tamanho reduzido
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// SHELL SORT
// =============================================================================

void Sorter::shellSort(std::vector<int>& v) {
    metrics_.reset();
    metrics_.maxRecursionDepth = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(v.size());

    // Sequência de gaps de Knuth: h = 1, 4, 13, 40, 121, 364, ...
    // Gerada por h = 3*h + 1 até h < n/3.
    // Complexidade com esta sequência: O(n^(4/3)) — melhor que O(n²).
    int gap = 1;
    while (gap < n / 3) gap = 3 * gap + 1;

    // Memória auxiliar: gap + índices + key — O(1)
    metrics_.auxMemBytes = 4 * sizeof(int);

    // Reduz o gap a cada passagem até gap = 1 (equivale ao Insertion Sort final)
    while (gap >= 1) {
        for (int i = gap; i < n; ++i) {
            int key = v[i];
            metrics_.memAccesses++;  // leitura de v[i]

            int j = i;
            while (j >= gap) {
                countComparison();
                if (v[j - gap] > key) {
                    v[j] = v[j - gap];
                    countMove();
                    j -= gap;
                } else {
                    break;
                }
            }
            v[j] = key;
            metrics_.memAccesses++;  // escrita na posição correta
            if (j != i) metrics_.swaps++;
        }
        gap /= 3;   // reduz o gap (sequência de Knuth inversa)
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// QUICK SORT — Particionamento de Lomuto
// =============================================================================

/**
 * Lomuto: pivô = v[hi].
 * Percorre v[lo..hi-1] mantendo dois "lados":
 *   v[lo..i]   → elementos ≤ pivô
 *   v[i+1..j]  → elementos > pivô
 * Ao final, coloca o pivô em v[i+1] e retorna i+1.
 */
int Sorter::partitionLomuto(std::vector<int>& v, int lo, int hi) {
    int pivot = v[hi];
    metrics_.memAccesses++;  // leitura do pivô

    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        countComparison();
        if (v[j] <= pivot) {
            ++i;
            std::swap(v[i], v[j]);
            countSwap();
        }
    }
    std::swap(v[i + 1], v[hi]);  // posiciona o pivô
    countSwap();
    return i + 1;
}

void Sorter::quickSortHelper(std::vector<int>& v, int lo, int hi, int depth) {
    if (depth > metrics_.maxRecursionDepth)
        metrics_.maxRecursionDepth = depth;

    if (lo >= hi) return;

    int p = partitionLomuto(v, lo, hi);
    quickSortHelper(v, lo,    p - 1, depth + 1);
    quickSortHelper(v, p + 1, hi,    depth + 1);
}

void Sorter::quickSort(std::vector<int>& v) {
    metrics_.reset();
    // Memória auxiliar = pilha de recursão implícita.
    // Profundidade média: O(log n) → ~log₂(n) * sizeof(frame) bytes.
    // Estimativa conservadora: depth_max * (3 * sizeof(int)) para lo, hi, p.
    metrics_.maxRecursionDepth = 0;
    metrics_.auxMemBytes       = 0;  // calculado após sort com profundidade real

    auto t0 = std::chrono::high_resolution_clock::now();

    if (!v.empty())
        quickSortHelper(v, 0, static_cast<int>(v.size()) - 1, 1);

    // Estima memória da pilha de recursão a posteriori
    metrics_.auxMemBytes = static_cast<long long>(metrics_.maxRecursionDepth)
                         * 3 * sizeof(int);  // lo, hi, p por frame

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}

// =============================================================================
// QUICK SORT RANDOMIZADO
// =============================================================================

/**
 * Igual ao Lomuto, mas troca um elemento aleatório com v[hi] antes de particionar.
 * Isso elimina o pior caso para entradas ordenadas/reversas sem alterar a lógica
 * de particionamento — apenas o pivô escolhido muda.
 */
int Sorter::partitionLomutoRandom(std::vector<int>& v, int lo, int hi) {
    // Escolhe pivô aleatório e o move para v[hi] (posição esperada pelo Lomuto)
    int randIdx = lcgRange(lo, hi);
    std::swap(v[randIdx], v[hi]);
    countSwap();  // conta a troca de pivô como movimentação

    return partitionLomuto(v, lo, hi);
}

void Sorter::quickSortRandomHelper(std::vector<int>& v, int lo, int hi, int depth) {
    if (depth > metrics_.maxRecursionDepth)
        metrics_.maxRecursionDepth = depth;

    if (lo >= hi) return;

    int p = partitionLomutoRandom(v, lo, hi);
    quickSortRandomHelper(v, lo,    p - 1, depth + 1);
    quickSortRandomHelper(v, p + 1, hi,    depth + 1);
}

void Sorter::quickSortRandom(std::vector<int>& v) {
    metrics_.reset();
    metrics_.maxRecursionDepth = 0;
    metrics_.auxMemBytes       = 0;

    // Reinicia o LCG para a seed original → garante reprodutibilidade:
    // quickSortRandom() com a mesma seed sempre escolhe os mesmos pivôs.
    lcgReset();

    auto t0 = std::chrono::high_resolution_clock::now();

    if (!v.empty())
        quickSortRandomHelper(v, 0, static_cast<int>(v.size()) - 1, 1);

    metrics_.auxMemBytes = static_cast<long long>(metrics_.maxRecursionDepth)
                         * 3 * sizeof(int);

    auto t1 = std::chrono::high_resolution_clock::now();
    metrics_.timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
}