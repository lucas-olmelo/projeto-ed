#ifndef DataGenerator_H
#define DataGenerator_H

#include <vector>

class GeradorDeVetor {
private:
    // Declaramos o gerador como um membro privado e estático
    static std::mt19937 gerador;
public:
    // Método 1: Vetor com valores aleatórios
    static std::vector<int> gerarValoresAleatorios(int tamanho);
    // Método 2: Vetor com valores aleatórios (reverse = true para ordem reversa)
    static std::vector<int> gerarValoresOrdenados(int tamanho, bool reverse);
    //Método 3: Vetor parcialmente ordenado
    static std::vector<int> gerarValoresParcialmenteOrdenados(int tamanho);
    //Método 4: Vetor com alta repetição
    static std::vector<int> gerarValoresAltamenteRepetidos(int tamanho);
    //Método 5: Vetor com Distribuição Gaussiana
    static std::vector<int> gerarValoresComDistribuicaoGaussiana(int tamanho, double media, double desvioPadrao);
};

#endif