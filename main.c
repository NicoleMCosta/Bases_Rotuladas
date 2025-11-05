#include "dist_euclidiana.h"

int main() {
    int num_combinacoes = 0;
    int qtdfiles = 0;
    FILE *csv1 = fopen("./bases/rotulada.csv", "r");
    if (csv1 == NULL) {
        printf("Erro ao reabrir distances.csv para leitura\n");
        return 1;
    }


    distancias(csv1, &num_combinacoes, &qtdfiles);
    printf("\nNúmero de combinações: %d\n", num_combinacoes);
    printf("Número de files: %d\n", qtdfiles);

    //limiar 0.102 = 1 componente tamanho 149(?)
    limiar(0.00102, num_combinacoes, "distances1.csv"); 
    componentes("limiar0.00102.csv");
    return 0;
}
