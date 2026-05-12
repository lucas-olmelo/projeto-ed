# 📊 Dashboard de Análise Empírica de Algoritmos

Este projeto é uma ferramenta de visualização de dados desenvolvida em **Python** utilizando **Streamlit**. O objetivo é contrastar a teoria da complexidade assintótica com o comportamento prático de algoritmos de ordenação e busca, servindo como suporte visual para o Relatório Técnico.

## 🛠️ 1. Inputs Necessários (Pré-requisitos)

Para que o código funcione, você precisa dos seguintes arquivos de base de dados no formato `.csv` no mesmo diretório do script:

1.  **`metricas_completa.csv`**: Dados dos algoritmos de ordenação (Bubble, Quick, Merge, etc.).
2.  **`metricas_busca_completa.csv`**: Dados dos algoritmos de busca (Sequencial e Binária).

**Estrutura esperada das colunas:**
- `Algoritmo`, `Distribuicao`, `TamanhoVetor`, `TempoNS`, `Comparacoes`, `Trocas`, `AcessosMemoria`, `MemoriaAuxiliarBytes`.

---

## 🚀 2. Como usar o Código

1.  **Instale as dependências:**
    ```bash
    pip install streamlit pandas plotly
    ```

2.  **Execute a aplicação:**
    ```bash
    streamlit run app.py
    ```

3.  **Acesso:**
    O Streamlit abrirá automaticamente no seu navegador (geralmente em `http://localhost:8501`).

---

## 🖱️ 3. Como interagir com o Dashboard

O controle é feito pela **Barra Lateral (Sidebar)**:
- **Tipo de Algoritmo:** Alterna entre as bases de "Ordenação" e "Busca".
- **Condição (Distribuição/Alvo):** Define o cenário (Aleatório, Crescente, Inexistente, etc.).
- **Tamanho do Vetor (N):** Define a escala do teste (ex: 1.000 ou 10.000).

---

## 📈 4. Como interpretar cada Gráfico

### A. Tempo Médio (ns)
- **O que mostra:** O tempo real de processamento.
- **Mapeamento:** Valida a **Seção 7.1**. Selecione *Ordenação -> Aleatório -> 10.000*. A barra do **Bubble Sort** será ordens de magnitude maior que a do **Quick Random**, provando a inviabilidade do tempo quadrático em grandes volumes.

### B. Acessos à Memória
- **O que mostra:** O esforço de comunicação com o hardware.
- **Mapeamento:** Valida a **Seção 3 (Modelo RAM)**. Demonstra que o tempo de execução é reflexo direto do volume de operações de leitura e escrita.

### C. Esforço: Trocas
- **O que mostra:** A quantidade de movimentações realizadas.
- **Mapeamento:** Valida a **Seção 7.2**. Ao filtrar *Ordenação -> Crescente*, verá as barras do **Insertion Sort** reduzirem drasticamente, validando sua natureza adaptativa $O(n)$ em vetores quase ordenados.

### D. Memória Auxiliar (Bytes)
- **O que mostra:** O custo de espaço extra.
- **Mapeamento:** Valida as **Seções 2 e 7.4**. Evidencia que o **Merge Sort** não é *in-place*, apresentando consumo muito superior ao Quick Sort ou Heap Sort.

---

## 📝 5. Tabelas de Referência
Ao final da página, o menu expansível permite auditar os números exatos e médias, facilitando a correlação com as tabelas estatísticas do Relatório Técnico.
