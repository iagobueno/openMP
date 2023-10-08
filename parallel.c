/* WSCAD - 9th Marathon of Parallel Programming
 * Simple Brute Force Algorithm for the
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <omp.h>

int min_distance;
int nb_towns;

// armazena informações sobre destinos das czzidades e suas distâncias.
typedef struct
{
    int to_town;
    int dist;
} d_info;

d_info **d_matrix; // usadas para armazenar informações sobre as distâncias entre cidades.
int *dist_to_origin;

// verifica se uma cidade já foi visitada em um caminho específico.
int present(int town, int depth, int *path)
{
    int i;
    for (i = 0; i < depth; i++)
        if (path[i] == town)
            return 1;
    return 0;
}

// Ela explora recursivamente todas as possíveis rotas, mas faz algumas podas
void tsp(int depth, int current_length, int *path)
{
    // Ignora caminhos cujo comprimento total já é maior do que a menor distância encontrada até agora.
    if (current_length >= min_distance)
        return;

    int i;

    // Se já visitou todas as cidades
    if (depth == nb_towns)
    {
        // O caminho atual é atualizado somando a distância da última cidade de volta à cidade de origem
        current_length += dist_to_origin[path[nb_towns - 1]];
        // Se o comprimento atual for menor que a menor encontrada, atualiza
        if (current_length < min_distance)
#pragma omp critical
        {
            min_distance = current_length;
        }
    }
    else
    {
        int town, me, dist;
        me = path[depth - 1]; // A cidade atual (a última cidade no caminho)
                              // Itera sobre todas as cidades para decidir qual será a próxima a ser visitada.

#pragma omp parallel for schedule(dynamic) num_threads(8) private(i, town, dist)
        for (i = 0; i < nb_towns; i++)
        {
            int *path_copy = (int *)malloc(sizeof(int) * nb_towns); // Cria uma cópia local do caminho
            memcpy(path_copy, path, sizeof(int) * nb_towns);

            town = d_matrix[me][i].to_town; // Próxima cidade candidata
            // Verifica se a cidade candidata ainda não foi visitada.
            if (!present(town, depth, path_copy))
            {
                path_copy[depth] = town;     // Adiciona a cidade candidata ao caminho.
                dist = d_matrix[me][i].dist; // Distância da cidade atual à cidade candidata.
                // Chama recursivamente a função tsp para a próxima cidade candidata.
                tsp(depth + 1, current_length + dist, path_copy);
            }

            free(path_copy); // Libera a cópia local do caminho
        }
    }
}

void greedy_shortest_first_heuristic(int *x, int *y)
{
    int i, j, k, dist;
    int *tempdist;

    tempdist = (int *)malloc(sizeof(int) * nb_towns);
    // Could be faster, albeit not as didactic.
    // Anyway, for tractable sizes of the problem it
    // runs almost instantaneously.
    for (i = 0; i < nb_towns; i++)
    {
        for (j = 0; j < nb_towns; j++)
        {
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            tempdist[j] = dx * dx + dy * dy;
        }
        for (j = 0; j < nb_towns; j++)
        {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++)
            {
                if (tempdist[k] < tmp)
                {
                    tmp = tempdist[k];
                    town = k;
                }
            }
            tempdist[town] = INT_MAX;
            d_matrix[i][j].to_town = town;
            dist = (int)sqrt(tmp);
            d_matrix[i][j].dist = dist;
            if (i == 0)
                dist_to_origin[town] = dist;
        }
    }

    free(tempdist);
}

// Lê as coordenadas das cidades, aloca memória e chama a função heurística.
void init_tsp()
{
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;

    st = scanf("%u", &nb_towns);
    if (st != 1)
        exit(1);

    d_matrix = (d_info **)malloc(sizeof(d_info *) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info *)malloc(sizeof(d_info) * nb_towns);
    dist_to_origin = (int *)malloc(sizeof(int) * nb_towns);

    x = (int *)malloc(sizeof(int) * nb_towns);
    y = (int *)malloc(sizeof(int) * nb_towns);

    for (i = 0; i < nb_towns; i++)
    {
        st = scanf("%u %u", x + i, y + i);
        if (st != 2)
            exit(1);
    }

    greedy_shortest_first_heuristic(x, y);

    free(x);
    free(y);
}

// aloca memória para o caminho, define a primeira cidade, executa o TSP e, em seguida, libera a memória.
int run_tsp()
{
    int i, *path;

    init_tsp();

    path = (int *)malloc(sizeof(int) * nb_towns);
    path[0] = 0;

    tsp(1, 0, path);

    free(path);
    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);
    free(d_matrix);

    return min_distance;
}

// lê o número de instâncias e executa o TSP para cada uma delas.
int main(int argc, char **argv)
{
    int num_instances, st;
    st = scanf("%u", &num_instances);
    if (st != 1)
        exit(1);
    while (num_instances-- > 0)
        printf("%d\n", run_tsp());
    return 0;
}
