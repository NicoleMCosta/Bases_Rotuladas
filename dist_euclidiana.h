#include "dataset.h"
#include <math.h>


void min_max_normalize(DistanciaPar arr[], int size) {
    if (size == 0) return;

    double min_val = arr[0].distancia;
    double max_val = arr[0].distancia;

    // Encontra os valores mínimo e máximo
    for (int i = 1; i < size; i++) {
        if (arr[i].distancia < min_val) {
            min_val = arr[i].distancia;
        }
        if (arr[i].distancia > max_val) {
            max_val = arr[i].distancia;
        }
    }

    // Aplica a normalização
    if (max_val - min_val == 0) {
        for (int i = 0; i < size; i++) {
            arr[i].distancia = 0.0;
        }
    } else {
        for (int i = 0; i < size; i++) {
            arr[i].distancia = (arr[i].distancia - min_val) / (max_val - min_val);
        }
    }
}

double distancia(Objetos dataA, Objetos dataB){
    return sqrt(pow(dataB.X - dataA.X,2) + pow(dataB.Y - dataA.Y,2.0) + pow(dataB.Z - dataA.Z, 2.0) + pow(dataB.w - dataA.w, 2.0));
}

void distancias(const char *filename, int *num_combinacoes, int *qtdfiles){
    (*qtdfiles)++;

    FILE *csvpt = fopen(filename, "r");
    if(!csvpt){
        printf("Erro ao abrir arquivo.\n");
        return;
    }
    
    Objetos *dados = openCSV(csvpt);

    *num_combinacoes = SIZE * (SIZE - 1) / 2;
    DistanciaPar *distancias = malloc(*num_combinacoes * sizeof(DistanciaPar));
    if (!distancias) {
        printf("Erro de memória ao alocar o array de distâncias\n");
        free(dados);
        fclose(csvpt);
    }

    // Calcula a distância para todos os pares
    int k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            distancias[k].ponto1 = i;
            distancias[k].ponto2 = j;
            distancias[k].distancia = distancia(dados[i], dados[j]);
            k++;
        }
    }
    
    min_max_normalize(distancias, *num_combinacoes);


    char nomefile[20];
    sprintf(nomefile, "distances%d.csv", *qtdfiles);
    FILE *output_csv = fopen(nomefile, "w");
    if (output_csv == NULL) {
        printf("Erro ao criar o arquivo distances%d.csv\n", *qtdfiles);
        free(distancias);
        free(dados);
        fclose(csvpt);
    }

    for (int i = 0; i < *num_combinacoes; i++) {
        fprintf(output_csv, "%d,%d,%f\n", distancias[i].ponto1 + 1, distancias[i].ponto2 + 1, distancias[i].distancia);
    }

    fclose(output_csv);
    fclose(csvpt);
    free(distancias);
    free(dados);
}