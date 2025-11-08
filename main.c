#include "dist_euclidiana.h"


void execute(FILE *csv1, int num_combinacoes, int *qtdfiles, char *nomefile){
    char *dist_file = distancias(csv1, &num_combinacoes, qtdfiles);
    if (dist_file == NULL) {
        printf("Erro ao gerar arquivo de distâncias.\n");
        free(dist_file);
        return;
    }

    float limiar_inicial = 0.11000;
    int count = 0;

    for (float lim = limiar_inicial; lim > 0.06f; lim -= 0.00001f) {
        // printf("loop %d (lim = %.5f)\n", count++, lim);

        char *arquivo_limiar = limiar(lim, num_combinacoes, dist_file, nomefile);
        if (arquivo_limiar == NULL) {
            printf("Erro ao criar arquivo de limiar.\n");
            break;
        }

        int compt = componentes(arquivo_limiar);
        free(arquivo_limiar);

        if (compt == 3) {
            printf("Limiar a ser usado: %.3f\n", lim);
            break;
        }
    }

}


int main() {
    int num_combinacoes = 0;
    int qtdfiles = 0;
    
    FILE *csv1 = fopen("./bases/rotulada.csv", "r");
    if (csv1 == NULL) {
        printf("Erro ao abrir base para leitura\n");
        return 1;
    }


    int num_combinacoes2 = 0;
    int qtdfiles2 = 0;

    FILE *csv2 = fopen("./bases/Dataset_rotulado_com_5_casos_de_proximidade.csv", "r");
    if (csv2 == NULL) {
        printf("Erro ao abrir base para leitura\n");
        return 1;
    }

    printf("LIMIAR BASE 1\n");
    execute(csv1, num_combinacoes, &qtdfiles, "limiar1.csv");

    printf("\n\n\nLIMIAR BASE 2\n");
    execute(csv2, num_combinacoes2, &qtdfiles2, "limiar2.csv");
    
    return 0;
}
