#include "dist_euclidiana.h"

int main() {
    int num_combinacoes = 0;
    int qtdfiles = 0;
    distancias("./bases/rotulada.csv", &num_combinacoes, &qtdfiles);
    printf("\nNúmero de combinações 1: %d\n", num_combinacoes);
    
    distancias("./bases/Dataset_rotulado_com_5_casos_de_proximidade.csv", &num_combinacoes, &qtdfiles);

    printf("\nNúmero de files: %d\n", qtdfiles);

    return 0;
}
