#include <iostream>
#include <string>
#include "DataGenerator.h"

using namespace std;

void imprimir(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
}

int main() {

    // Chamando os métodos estáticos sem precisar instanciar a classe
    auto vetorA = GeradorDeVetor::gerarValoresParcialmenteOrdenados(10);
    std::cout << "parc ordenado: ";
    imprimir(vetorA);    

    return 0;
}