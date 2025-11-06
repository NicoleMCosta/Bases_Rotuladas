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


Objetos* openCSV(FILE *csv) {
    if (!csv) {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }

    Objetos *obj = malloc(SIZE * sizeof(Objetos));
    if (!obj) {
        printf("Erro de memória\n");
        return NULL;
    }

    char linha[128];
    int caso = 0;

    while (fgets(linha, sizeof(linha), csv) && caso < SIZE) {
        // remove quebra de linha
        linha[strcspn(linha, "\r\n")] = 0;

        // lê os 4 floats e o restante da linha (inclusive espaços) como tipo
        if (sscanf(linha, "%f,%f,%f,%f,%[^\n]", 
                   &obj[caso].X, &obj[caso].Y, &obj[caso].Z, &obj[caso].w, obj[caso].tipo) == 5) {
            caso++;
        }
    }

    printf("Total de objetos lidos: %d\n", caso);
    return obj;
}


char *limiar(float limiar, int num_combinacoes, const char *dist_file){
    DistanciaPar dados;

    FILE *csv = fopen(dist_file, "r");
    if (csv == NULL) {
        printf("Erro ao reabrir distances.csv para leitura\n");
    }

    char nomefile[20];
    sprintf(nomefile, "limiar%.5f.csv", limiar);
    FILE *limiarcalc= fopen(nomefile, "w");
        if(limiarcalc == NULL){
            printf("Erro ao criar o arquivo %s\n", nomefile);
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
    
    char *ret = malloc(strlen(nomefile) + 1);
    if (ret == NULL) {
        printf("Erro de memória ao salvar nome do arquivo.\n");
        return NULL;
    }
    strcpy(ret, nomefile);
    return ret;
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
int componentes(const char *filename){
    FILE *csv = fopen(filename, "r");
    if(!csv){
        printf("Erro ao abrir arquivo. COMPONENTES\n");
        return 1;
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
    
    return num_comps;
}

