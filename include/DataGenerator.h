#ifndef DataGenerator_H
#define DataGenerator_H

#include <vector>
#include <random>

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
};

#endif