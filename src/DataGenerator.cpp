#include "DataGenerator.h"
#include <random>
#include <iostream>

using namespace std;

const int SEED = 42;

//mt19937 GeradorDeVetor::gerador(std::random_device{}());
mt19937 GeradorDeVetor::gerador(SEED);

vector<int> GeradorDeVetor::gerarValoresAleatorios(int tamanho) {
    vector<int> vetorAleatorio(tamanho);

    uniform_int_distribution<int> dist(0, tamanho*10);
    
    for (int& i : vetorAleatorio) {
        i = dist(gerador);
    }

    return vetorAleatorio;
}

vector<int> GeradorDeVetor::gerarValoresOrdenados(int tamanho, bool reverse) {
    vector<int> vetorOrdenado(tamanho);

    if (!reverse)
    {
        for (int i = 0; i < tamanho; i++)
        {
            vetorOrdenado[i] = i;
        }
    } else {
        for (int i = tamanho; i > 0; i--)
        {
            vetorOrdenado[tamanho - i] = i;
        }
    }
    
    return vetorOrdenado;
}

vector<int> GeradorDeVetor::gerarValoresParcialmenteOrdenados(int tamanho) {
    vector<int> vetorParcialmenteOrdenado = gerarValoresOrdenados(tamanho, false);

    int quantidadeDesordenado = tamanho * 0.1;
    uniform_int_distribution<int> dist(0, tamanho - 1);

    for (int i = 0; i < quantidadeDesordenado; i++)
    {
        int pos1 = dist(gerador);
        int pos2 = dist(gerador);

        std::swap(vetorParcialmenteOrdenado[pos1], vetorParcialmenteOrdenado[pos2]);
    }
    
    return vetorParcialmenteOrdenado;
}

vector<int> GeradorDeVetor::gerarValoresAltamenteRepetidos(int tamanho){

    vector<int> vetorAltamenteRepetido(tamanho);

    uniform_int_distribution<int> dist(0, tamanho * 0.2);

    for (int& i : vetorAltamenteRepetido) {
        i = dist(gerador);
    }

    return vetorAltamenteRepetido;
}

vector<int> GeradorDeVetor::gerarValoresComDistribuicaoGaussiana(int tamanho, double media, double desvioPadrao){

    vector<int> vetorDistGaussiana(tamanho);

    std::normal_distribution<double> dist(media, desvioPadrao);

    for (int& i : vetorDistGaussiana) {
        i = dist(gerador);
    }

    return vetorDistGaussiana;
}