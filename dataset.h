#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 150

typedef struct Objetos{
    float X;
    float Y;
    float Z;
    float w;
    char tipo[7];
}Objetos;

typedef struct {
    int ponto1;
    int ponto2;
    double distancia;
} DistanciaPar;


Objetos* openCSV(FILE *csv){
    if(csv == NULL){
        printf("Erro ao abrir arquivo DATASET\n");
        return NULL;
    }
    
    Objetos *obj = malloc((SIZE+1)*sizeof(Objetos));
    if (!obj) {
        printf("Erro de memoria\n");
        return NULL;
    }

    int r = 5;
    Objetos temp;
    int caso = 0;
    while ((r = fscanf(csv, "%f,%f,%f,%f,%s", &temp.X, &temp.Y, &temp.Z, &temp.w, temp.tipo)) == 5) {
        obj[caso++] = temp;
    }

    return obj;
}

int limiar(float limiar, int num_combinacoes, const char *dist_file){
    DistanciaPar dados;

    FILE *csv = fopen(dist_file, "r");
    if (csv == NULL) {
        printf("Erro ao reabrir distances.csv para leitura\n");
        return 1;
    }

    char nomefile[20];
    sprintf(nomefile, "limiar%.1f.csv", limiar);
    FILE *limiarcalc= fopen(nomefile, "w");
        if(limiarcalc == NULL){
            printf("Erro ao criar o arquivo %s\n", nomefile);
            return 1;
        }

    int count = 0;
    while(fscanf(csv, "%d,%d,%lf", &dados.ponto1, &dados.ponto2, &dados.distancia) == 3){
        if(dados.distancia <= limiar){
                fprintf(limiarcalc, "%d,%d\n", dados.ponto1, dados.ponto2);
                count++;
            }
        if (count >= num_combinacoes)
            break;
    }
    fclose(limiarcalc);
    fclose(csv);
    return 0;
}


int matriz[SIZE][SIZE];
int visitado[SIZE];
int vertices[SIZE];
int n_vertices = 0;

int indiceVertice(int rotulo){
    for(int i = 0; i < n_vertices; i++){
        if (vertices[i] == rotulo)
            return i;
    }
    vertices[n_vertices] = rotulo;
    return n_vertices++;
}

void dfs(int v, int *tamanho){
    visitado[v] = 1;
    (*tamanho)++;

    for(int i = 0; i < n_vertices; i++){
        if(matriz[v][i] == 1 && visitado[i] == 0){
            dfs(i, tamanho);
        }
    }
}


//modificar função abaixo para contar e popular vetores de cada tipo existente em cada componente????
void componentes(const char *filename){
    FILE *csv = fopen(filename, "r");
    if(!csv){
        printf("Erro ao abrir arquivo. COMPONENTES\n");
        return;
    }

    n_vertices = 0;
    for (int i = 0; i < SIZE; i++) {
        vertices[i] = 0;
        visitado[i] = 0;
        for (int j = 0; j < SIZE; j++) {
            matriz[i][j] = 0;
        }
    }

    for(int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++)
            matriz[i][j] = 0;
        visitado[i] = 0;
    }

    int p1, p2;

    while(fscanf(csv, "%d,%d", &p1, &p2) == 2){
        int a = indiceVertice(p1);
        int b = indiceVertice(p2);
        matriz[a][b] = 1;
        matriz[b][a] = 1;
    }

    fclose(csv);

    int num_comps = 0;
    int tamanhos[n_vertices];

    for(int i = 0; i < n_vertices; i++){
        if(visitado[i] == 0){
            int tamanho = 0;
            dfs(i, &tamanho);
            tamanhos[num_comps++] = tamanho;
        }
    }

    printf("Número de componentes conexos: %d\n", num_comps);
    for (int i = 0; i < num_comps; i++)
        printf("Tamanho do componente %d: %d\n", i + 1, tamanhos[i]);
}

