#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 150

typedef struct Objetos
{
    float X;
    float Y;
    float Z;
    float w;
    char tipo[10];
} Objetos;

typedef struct
{
    int ponto1;
    int ponto2;
    double distancia;
} DistanciaPar;

typedef struct
{
    int idOriginal;
    int tamanho;
    double sx, sy, sz, sw;
    double cx, cy, cz, cw;
    int votos[3];
    int tipoPred;
} ClusterML;

Objetos *openCSV(FILE *csv)
{
    if (!csv)
    {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }
    Objetos *obj = malloc(SIZE * sizeof(Objetos));
    char linha[128];
    int caso = 0;
    while (fgets(linha, sizeof(linha), csv) && caso < SIZE)
    {
        linha[strcspn(linha, "\r\n")] = 0;
        if (sscanf(linha, "%f,%f,%f,%f,%[^\n]",
                   &obj[caso].X, &obj[caso].Y, &obj[caso].Z, &obj[caso].w, obj[caso].tipo) >= 4)
        {
            if (strlen(obj[caso].tipo) == 0)
                strcpy(obj[caso].tipo, "N/A");
            caso++;
        }
    }
    return obj;
}

char *limiar(float limiar, int num_combinacoes, const char *dist_file, char *nomefile)
{
    DistanciaPar dados;
    FILE *csv = fopen(dist_file, "r");
    if (!csv)
        return NULL;
    FILE *out = fopen(nomefile, "w");
    if (!out)
    {
        fclose(csv);
        return NULL;
    }

    int count = 0;
    while (fscanf(csv, "%d,%d,%lf", &dados.ponto1, &dados.ponto2, &dados.distancia) == 3)
    {
        if (dados.distancia <= limiar)
        {
            fprintf(out, "%d,%d\n", dados.ponto1, dados.ponto2);
            count++;
        }
        if (count >= num_combinacoes)
            break;
    }
    fclose(out);
    fclose(csv);
    char *ret = malloc(strlen(nomefile) + 1);
    if (ret)
        strcpy(ret, nomefile);
    return ret;
}

// Globais para DFS
int matriz[SIZE][SIZE];
int visitado[SIZE];
int n_vertices = SIZE;

void dfs_simples(int v, int *tamanho)
{
    visitado[v] = 1;
    (*tamanho)++;
    for (int i = 0; i < SIZE; i++)
    {
        if (matriz[v][i] && !visitado[i])
            dfs_simples(i, tamanho);
    }
}

int componentes(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return 1;
    for (int i = 0; i < SIZE; i++)
    {
        visitado[i] = 0;
        for (int j = 0; j < SIZE; j++)
            matriz[i][j] = 0;
    }
    int p1, p2;
    while (fscanf(f, "%d,%d", &p1, &p2) == 2)
    {
        matriz[p1 - 1][p2 - 1] = matriz[p2 - 1][p1 - 1] = 1;
    }
    fclose(f);

    int num_c = 0, tamanhos[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        if (!visitado[i])
        {
            int t = 0;
            dfs_simples(i, &t);
            tamanhos[num_c++] = t;
        }
    }

    int m3 = 0;
    for (int i = 0; i < num_c; i++)
        if (tamanhos[i] >= 14)
            m3++;
    if (m3 >= 3)
    {

        int top[3] = {0};
        for (int i = 0; i < num_c; i++)
        {
            if (tamanhos[i] > top[0])
            {
                top[2] = top[1];
                top[1] = top[0];
                top[0] = tamanhos[i];
            }
            else if (tamanhos[i] > top[1])
            {
                top[2] = top[1];
                top[1] = tamanhos[i];
            }
            else if (tamanhos[i] > top[2])
            {
                top[2] = tamanhos[i];
            }
        }
        if ((top[0] - top[2]) <= 15)
        {
            printf("Componentes: %d | Tamanhos Top 3: %d, %d, %d\n", num_c, top[0], top[1], top[2]);
            return 3;
        }
    }
    return 1;
}



int tipo_para_id(char *t)
{
    if (strncmp(t, "Tipo 1", 6) == 0)
        return 0;
    if (strncmp(t, "Tipo 2", 6) == 0)
        return 1;
    if (strncmp(t, "Tipo 3", 6) == 0)
        return 2;
    return -1;
}

char *id_para_tipo(int id)
{
    if (id == 0)
        return "Tipo 1";
    if (id == 1)
        return "Tipo 2";
    if (id == 2)
        return "Tipo 3";
    return "N/A";
}

void balancear_pontos(Objetos *d, int n)
{
    float min[4] = {1e9, 1e9, 1e9, 1e9}, max[4] = {-1e9, -1e9, -1e9, -1e9};
    for (int i = 0; i < n; i++)
    {
        if (d[i].X < min[0])
            min[0] = d[i].X;
        if (d[i].X > max[0])
            max[0] = d[i].X;
        if (d[i].Y < min[1])
            min[1] = d[i].Y;
        if (d[i].Y > max[1])
            max[1] = d[i].Y;
        if (d[i].Z < min[2])
            min[2] = d[i].Z;
        if (d[i].Z > max[2])
            max[2] = d[i].Z;
        if (d[i].w < min[3])
            min[3] = d[i].w;
        if (d[i].w > max[3])
            max[3] = d[i].w;
    }
    for (int i = 0; i < n; i++)
    {
        d[i].X = (d[i].X - min[0]) / (max[0] - min[0]);
        d[i].Y = (d[i].Y - min[1]) / (max[1] - min[1]);
        d[i].Z = (d[i].Z - min[2]) / (max[2] - min[2]);
        d[i].w = (d[i].w - min[3]) / (max[3] - min[3]);
    }
}

void dfs_ml(int v, int c_id, int *assign, ClusterML *c, Objetos *d)
{
    visitado[v] = 1;
    assign[v] = c_id;
    c[c_id].tamanho++;
    c[c_id].sx += d[v].X;
    c[c_id].sy += d[v].Y;
    c[c_id].sz += d[v].Z;
    c[c_id].sw += d[v].w;
    int tid = tipo_para_id(d[v].tipo);
    if (tid != -1)
        c[c_id].votos[tid]++;
    for (int i = 0; i < SIZE; i++)
        if (matriz[v][i] && !visitado[i])
            dfs_ml(i, c_id, assign, c, d);
}

int cmp_ml(const void *a, const void *b)
{
    return ((ClusterML *)b)->tamanho - ((ClusterML *)a)->tamanho;
}

void imprimir_metricas(int cm[3][3])
{
    int total = 0;
    double acc_g = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            total += cm[i][j];
    printf("\n--- Relatório Final ML ---\nMatriz de Confusão (L=Real, C=Predito):\n\tP_T1\tP_T2\tP_T3\n");
    for (int i = 0; i < 3; i++)
        printf("R_%s\t%d\t%d\t%d\n", id_para_tipo(i), cm[i][0], cm[i][1], cm[i][2]);
    printf("\nMétricas por Classe:\n");
    for (int i = 0; i < 3; i++)
    {
        int tp = cm[i][i], fn = 0, fp = 0, tn;
        for (int j = 0; j < 3; j++)
            if (i != j)
                fn += cm[i][j];
        for (int j = 0; j < 3; j++)
            if (i != j)
                fp += cm[j][i];
        tn = total - (tp + fn + fp);
        acc_g += tp;
        double p = (tp + fp) > 0 ? (double)tp / (tp + fp) : 0;
        double r = (tp + fn) > 0 ? (double)tp / (tp + fn) : 0;
        double f1 = (p + r) > 0 ? 2 * (p * r) / (p + r) : 0;
        printf("[%s] TP:%d FP:%d FN:%d | Precision: %.3f, Recall: %.3f, F1: %.3f\n", id_para_tipo(i), tp, fp, fn, p, r, f1);
    }
    printf("=> Acurácia Global: %.2f%%\n---------------------------------\n", (acc_g / total) * 100.0);
}

void realizar_analise_ml(const char *fbase, const char *fgrafo, int balancear)
{
    Objetos *dados = openCSV(fopen(fbase, "r"));
    if (!dados)
        return;
    if (balancear)
        balancear_pontos(dados, SIZE);

    FILE *fg = fopen(fgrafo, "r");
    if (!fg)
    {
        free(dados);
        return;
    }
    for (int i = 0; i < SIZE; i++)
    {
        visitado[i] = 0;
        for (int j = 0; j < SIZE; j++)
            matriz[i][j] = 0;
    }
    int p1, p2;
    while (fscanf(fg, "%d,%d", &p1, &p2) == 2)
        matriz[p1 - 1][p2 - 1] = matriz[p2 - 1][p1 - 1] = 1;
    fclose(fg);

    int assign[SIZE], num_c = 0;
    ClusterML c[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        if (!visitado[i])
        {
            c[num_c] = (ClusterML){.idOriginal = num_c, .tamanho = 0, 0};
            dfs_ml(i, num_c, assign, c, dados);
            num_c++;
        }
    }

    qsort(c, num_c, sizeof(ClusterML), cmp_ml);
    int k = (num_c < 3) ? num_c : 3;
    int top3_orig_ids[3] = {-1, -1, -1};

    printf("Top 3 Clusters (Votação):\n");
    for (int i = 0; i < k; i++)
    {
        top3_orig_ids[i] = c[i].idOriginal;
        c[i].cx = c[i].sx / c[i].tamanho;
        c[i].cy = c[i].sy / c[i].tamanho;
        c[i].cz = c[i].sz / c[i].tamanho;
        c[i].cw = c[i].sw / c[i].tamanho;
        int max = -1, win = -1;
        for (int v = 0; v < 3; v++)
            if (c[i].votos[v] > max)
            {
                max = c[i].votos[v];
                win = v;
            }
        c[i].tipoPred = win;
        printf("  #%d (Tam:%d) -> Rótulo: %s\n", i + 1, c[i].tamanho, id_para_tipo(win));
    }

    int cm[3][3] = {{0}}, reatribuidos = 0;
    for (int i = 0; i < SIZE; i++)
    {
        int real = tipo_para_id(dados[i].tipo);
        if (real == -1)
            continue;

        int predito = -1;

        for (int j = 0; j < k; j++)
        {
            if (assign[i] == top3_orig_ids[j])
            {
                predito = c[j].tipoPred;
                break;
            }
        }

        if (predito == -1)
        {
            double min_dist = 1e9;
            int best_c = -1;
            for (int j = 0; j < k; j++)
            {
                double d = sqrt(pow(dados[i].X - c[j].cx, 2) + pow(dados[i].Y - c[j].cy, 2) +
                                pow(dados[i].Z - c[j].cz, 2) + pow(dados[i].w - c[j].cw, 2));
                if (d < min_dist)
                {
                    min_dist = d;
                    best_c = j;
                }
            }
            if (best_c != -1)
            {
                predito = c[best_c].tipoPred;

                printf("  -> Ponto %d (Tipo Real: %s) reatribuído para Cluster #%d (%s) - Dist: %.4f\n",
                       i + 1, dados[i].tipo, best_c + 1, id_para_tipo(predito), min_dist);

                reatribuidos++;
            }
        }
        if (predito != -1)
            cm[real][predito]++;
    }
    printf("Pontos reatribuídos por centroide (estavam fora dos Top 3): %d\n", reatribuidos);
    imprimir_metricas(cm); 
    free(dados);
}