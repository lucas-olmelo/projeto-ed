# Projeto de Análise de Algoritmos (Estrutura de Dados)

Este projeto tem como objetivo a implementação, instrumentação e análise estatística de diversos algoritmos de ordenação e busca em C++. O sistema gera diferentes cenários de dados para testar a eficiência dos algoritmos em termos de tempo e uso de recursos.

## 🚀 Estrutura do Projeto

A arquitetura do projeto foi desenhada utilizando Programação Orientada a Objetos para garantir a separação de responsabilidades e facilitar a manutenção.

```text
projeto_ed1/
├── README.md             # Documentação do projeto
├── .gitignore            # Arquivos ignorados pelo Git (build, exe, etc)
├── include/              # Cabeçalhos (.h) - Declarações
│   ├── DataGenerator.h   # Protótipos para geração de vetores
│   ├── Sorter.h          # Protótipos dos algoritmos de ordenação
│   ├── Searcher.h        # Protótipos dos algoritmos de busca
│   ├── Metrics.h         # Estrutura de dados para instrumentação
│   └── ExperimentRunner.h# Orquestrador de testes estatísticos
└── src/                  # Implementações (.cpp) - Lógica
    ├── main.cpp          # Ponto de entrada do programa
    ├── DataGenerator.cpp # Lógica de geração (Uniforme, Ordenada, etc)
    ├── Sorter.cpp        # Implementação dos Sorts (Bubble, Quick, etc)
    ├── Searcher.cpp      # Implementação das Buscas (Sequencial, Binária)
    ├── Metrics.cpp       # Lógica de reset e manipulação de métricas
    └── ExperimentRunner.cpp # Lógica de repetição (30x) e médias
```

## 📋 Componentes Principais

### 1. DataGenerator
Responsável por criar os vetores de teste. Utiliza um **Gerador Congruente Linear (LCG)** manual com uma **seed fixa (ex: 42)** para garantir que os resultados sejam 100% reprodutíveis em qualquer máquina.
* **Cenários:** Uniforme, Ordenado, Reverso, Parcialmente Ordenado, Alta Repetição e Distribuição Gaussiana.

### 2. Metrics (Instrumentação)
Estrutura que armazena os dados de desempenho de cada execução:
* Número de comparações de chaves.
* Número de trocas/movimentações de elementos.
* Total de acessos à memória (leitura/escrita).
* Tempo de execução em nanossegundos (`std::chrono`).
* Profundidade de recursão e memória auxiliar.

### 3. Sorter & Searcher
Módulos que contêm as implementações dos algoritmos. Cada função recebe uma referência para um objeto `Metrics`, permitindo que a instrumentação ocorra de forma transparente durante a execução do algoritmo.

### 4. ExperimentRunner
Gerencia a complexidade da **PARTE 3** do projeto. Ele automatiza o loop de 30 execuções para cada combinação de (Tamanho x Cenário x Algoritmo) e calcula as médias e a dispersão dos dados.

## 🛠️ Como Compilar e Rodar

Rodar os comandos:

1.  Powershell:
    ```bash
    g++ -I include src/*.cpp -o programa ; .\programa.exe
    ```
2.  CMD:
    ```bash
    g++ -I include src/*.cpp -o programa && programa.exe
    ```
3.  Terminal (Mac / Linux)::
    ```bash
    g++ -I include src/*.cpp -o programa && ./programa
    ```

## ⚙️ Requisitos
* Compilador C++ com suporte ao padrão **C++17** (GCC 7+, Clang 5+, ou MSVC 2017+).