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
    auto vetorA = GeradorDeVetor::gerarValoresComDistribuicaoGaussiana(1000, 50, 5);
    std::cout << "Dist Normal: ";
    imprimir(vetorA);

    return 0;
}