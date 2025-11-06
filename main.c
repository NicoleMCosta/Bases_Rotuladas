#include "dist_euclidiana.h"


void execute(FILE *csv1, int num_combinacoes, int *qtdfiles){
        char *dist_file = distancias(csv1, &num_combinacoes, qtdfiles);
        if (dist_file == NULL) {
            printf("Erro ao gerar arquivo de distâncias.\n");
            free(dist_file);
            return;
        }

    // float lim = 0.150;
    // int count = 0;
    // for(float i = lim; i < 0.0; i--){
    //     printf("loop %i %lf\n", count++,lim);

    //     char *arquivo_limiar = limiar(lim, num_combinacoes, dist_file);
    //     if (arquivo_limiar == NULL) {
    //         printf("Erro ao criar limiar file\n");
    //         free(arquivo_limiar);
    //         return;
    //     }

    //     int compt = componentes(arquivo_limiar);  
    //     if(compt == 3){
    //         printf("Limiar a ser usado: %lf", lim);
    //         break;
    //     }else{
    //         lim= lim - 0.001;
    //     }

    // }

    float limiar_inicial = 0.150;
int count = 0;

for (float lim = limiar_inicial; lim > 0.0f; lim -= 0.001f) {
    printf("loop %d (lim = %.3f)\n", count++, lim);

    char *arquivo_limiar = limiar(lim, num_combinacoes, dist_file);
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

    execute(csv1, num_combinacoes, &qtdfiles);
    execute(csv2, num_combinacoes2, &qtdfiles2);
    
    return 0;
}
