#include "ExperimentRunner.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Metrics ExperimentRunner::runSortExperiment(
    const std::string& nomeAlgoritmo,
    const std::string& nomeCenario,
    SortFunction algoritmo,
    GeneratorFunction gerador,
    int tamanho,
    int numExecucoes) 
{
    Metrics mediaMetrics;
    Sorter ordenador(42); // Seed fixa inicial

    std::cout << "[EXECUTANDO] " << nomeAlgoritmo << " | Cenario: " << nomeCenario 
              << " | N: " << tamanho << " | Execucoes: " << numExecucoes;


    for (int i = 0; i < numExecucoes; i++) {
        // 1. Gera o vetor para esta execução
        std::vector<int> vetor = gerador(tamanho);

        // 2. Executa a ordenação
        algoritmo(ordenador, vetor);

        // 3. Coleta e acumula as métricas
        Metrics metricaAtual = ordenador.copyMetrics();

        // 4. Salva a execução individual no arquivo "bruto"
        saveMetricsToCSV("results/ordenacao/metricas_sort_completa.csv", nomeAlgoritmo, nomeCenario, tamanho, metricaAtual, i + 1);

        mediaMetrics.comparisons += metricaAtual.comparisons;
        mediaMetrics.swaps += metricaAtual.swaps;
        mediaMetrics.memAccesses += metricaAtual.memAccesses;
        mediaMetrics.timeNs += metricaAtual.timeNs;
        
        // Memória auxiliar e recursão máxima
        if (metricaAtual.maxRecursionDepth > mediaMetrics.maxRecursionDepth) {
            mediaMetrics.maxRecursionDepth = metricaAtual.maxRecursionDepth;
        }
        if (metricaAtual.auxMemBytes > mediaMetrics.auxMemBytes) {
            mediaMetrics.auxMemBytes = metricaAtual.auxMemBytes;
        }
    }

    // 5. Calcula as médias
    mediaMetrics.comparisons /= numExecucoes;
    mediaMetrics.swaps /= numExecucoes;
    mediaMetrics.memAccesses /= numExecucoes;
    mediaMetrics.timeNs /= numExecucoes;

    saveMetricsToCSV("results/ordenacao/metricas_sort_medias.csv", nomeAlgoritmo, nomeCenario, tamanho, mediaMetrics);

    std::cout << " [FINALIZADO]" << "\n";

    return mediaMetrics;
}

Metrics ExperimentRunner::runSearchExperiment(
    const std::string& nomeAlgoritmo,
    const std::string& nomeCenario,
    SearchFunction algoritmo,
    std::vector<int>& vetorBase,
    int valorBusca,
    int numExecucoes) 
{
    Metrics mediaMetrics;
    Searcher searcher;

    std::cout << "[EXECUTANDO] " << nomeAlgoritmo << " | Cenario: " << nomeCenario 
              << " | N: " << vetorBase.size() << " | Alvo: " << valorBusca << " | Execucoes: " << numExecucoes;

    for (int i = 0; i < numExecucoes; i++) {
        // Executa a busca
        algoritmo(searcher, vetorBase, valorBusca);
        Metrics atual = searcher.copyMetrics();

        saveMetricsToCSV("results/busca/metricas_busca_completa.csv", nomeAlgoritmo, nomeCenario, vetorBase.size(), atual, i + 1);

        mediaMetrics.comparisons += atual.comparisons;
        mediaMetrics.memAccesses += atual.memAccesses;
        mediaMetrics.timeNs += atual.timeNs;
        if (atual.maxRecursionDepth > mediaMetrics.maxRecursionDepth) mediaMetrics.maxRecursionDepth = atual.maxRecursionDepth;
        if (atual.auxMemBytes > mediaMetrics.auxMemBytes) mediaMetrics.auxMemBytes = atual.auxMemBytes;
    }

    // Calcula médias
    mediaMetrics.comparisons /= numExecucoes;
    mediaMetrics.memAccesses /= numExecucoes;
    mediaMetrics.timeNs /= numExecucoes;

    // Salva a média
    saveMetricsToCSV("results/busca/metricas_busca_medias.csv", nomeAlgoritmo, nomeCenario, vetorBase.size(), mediaMetrics);

    std::cout << " [FINALIZADO]" << "\n";

    return mediaMetrics;
}

void ExperimentRunner::saveMetricsToCSV(
    const std::string& nomeArquivo, 
    const std::string& algoritmo, 
    const std::string& distribuicao,
    int tamanho,
    const Metrics& m,
    int execucaoId) 
{
    std::filesystem::path caminhoArquivo(nomeArquivo);
    if (caminhoArquivo.has_parent_path()) {
        std::filesystem::create_directories(caminhoArquivo.parent_path());
    }

    std::ifstream checarArquivo(nomeArquivo);
    bool novoArquivo = !checarArquivo.is_open();
    checarArquivo.close();

    std::ofstream arquivo(nomeArquivo, std::ios::app);

    if (arquivo.is_open()) {
        if (novoArquivo) {
            arquivo << "Algoritmo,Distribuicao,TamanhoVetor,Comparacoes,Trocas,AcessosMemoria,ProfRecursao,TempoNS,MemoriaAuxiliarBytes\n";
        }
        arquivo << algoritmo << ","
                << distribuicao << ","
                << tamanho << ","
                << m.comparisons << ","
                << m.swaps << ","
                << m.memAccesses << ","
                << m.maxRecursionDepth << ","
                << m.timeNs << ","
                << m.auxMemBytes << "\n";
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo para exportacao: " << nomeArquivo << std::endl;
    }
}