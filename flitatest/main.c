#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#define SWAP(a,b) a += b; \
                  b = a-b; \
                  a = a - b;

typedef struct {
    int node;
    int** matrix;
} Graph;

int countNodes(FILE *file){
    int numOfNodes = 0;
    int first, second;
    while (fscanf(file, "%d %d", &first, &second) != EOF) {
        if (first > numOfNodes)
            numOfNodes = first;
        if (second > numOfNodes)
            numOfNodes = second;
    }
    return numOfNodes;
}

Graph* initializeGraph(int nodes) {
    Graph *n_g = malloc(sizeof(Graph));
    n_g->matrix = malloc(nodes * sizeof(int*));
    n_g->node = nodes;
    for(int i = 0; i < nodes; i++) {
        n_g->matrix[i] = calloc(nodes, sizeof(int));
    }
    return n_g;
}

void createGraph(Graph* graph, FILE* file1) {
    int first, second;
    int startNode = 0;
    while (fscanf(file1, "%d %d", &first, &second) != EOF) {
        if (first > startNode)
            startNode = first;
        if (second > startNode)
            startNode = second;
        graph->matrix[first - 1][second - 1] = 1;
        graph->matrix[second - 1][first - 1] = 1;
    }
}

void printGraph(Graph* graph) {
    for(int i = 0; i < graph->node; i++) {
        for(int j = 0; j < graph->node; j++) {
            printf("%d ", graph->matrix[i][j]);
        }
        putchar('\n');
    }
}

void saveAsDot(Graph* graph) {
    FILE* file2 = fopen("forgraph.dot", "w");
    fputs("graph{\n", file2);
    for(int i = 0; i < graph->node; i++) {
        fprintf(file2, "\t%d;\n", i + 1);
    }
    for(int i = 0; i < graph->node; i++) {
        for(int j = i; j < graph->node; j++) {
            if(graph->matrix[i][j] == 1) {
                fprintf(file2, "\t%d -- %d;\n", i + 1, j + 1);
            }
        }
    }
    fputs("}\n", file2);
    fclose(file2);
    system("dot -Tpng -O forgraph.dot");
}

void deleteNode(Graph *graph, int nodeToDelete) {
    if (nodeToDelete < 1 || nodeToDelete > graph->node) {
        puts("Invalid node number");
        return;
    }
    int **matrix = (int**) malloc(sizeof(int*) * (graph->node - 1));
    for(int i = 0, i_new = 0; i < graph->node; i++, i_new++) {
        if (i == nodeToDelete - 1) {
            i_new--;
            continue;
        }
        matrix[i_new] = (int*) malloc(sizeof(int) * (graph->node - 1));
        for (int j = 0, j_new = 0; j < graph->node; j++, j_new++) {
            if (j == nodeToDelete - 1) {
                j_new--;
                continue;
            }
            matrix[i_new][j_new] = graph->matrix[i][j];
        }
    }
    for(int i = 0; i < graph->node; i++) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    graph->matrix = matrix;
    graph->node--;
}

void sortGraph(Graph *graph){
    int degrees[graph->node];
    for(int i = 0; i < graph->node; i++){
        int degree = 0;
        for(int j = 0; j < graph->node; j++){
            degree += graph->matrix[i][j];
        }
        degrees[i] = degree;
    }
    for(int i = 0; i < graph->node - 1; i++){
        for(int j = i + 1; j < graph->node; j++){
            if(degrees[i] < degrees[j]){
                SWAP(degrees[i], degrees[j]);
                for(int k = 0; k < graph->node; k++){
                    SWAP(graph->matrix[k][i], graph->matrix[k][j]);
                }
                for(int k = 0; k < graph->node; k++){
                    SWAP(graph->matrix[i][k], graph->matrix[j][k]);
                }
            }
        }
    }
}
void freeGraph(Graph* graph){
    for(int i = 0; i < graph->node; i++) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
}

int main() {
    FILE *fp = fopen("..\\time.txt", "w");
    int startTime; // Время работы программы замеряем как разницу между временем начала программы и временем её окончания
    int endTime;

    if (fp == NULL) {
        puts("Can't open file time.txt for writing");
        exit(EXIT_FAILURE);
    }

    for (int i = START_DATA_SIZE; i <= END_DATA_SIZE; i = i + RANDOM_DATA_SIZE_STEP) {
        GiveRandFile1(i);
        startTime = clock();

        FILE *file1 = fopen("forgraph.txt", "r");
        if (file1 == NULL) {
            puts("File not found");
            return 1;
        }

        int nodes = countNodes(file1);
        Graph *graph = initializeGraph(nodes);
        fclose(file1);
        FILE *file2 = fopen("forgraph.txt", "r");
        if (file2 == NULL) {
            puts("File not found");
            return 1;
        }
        createGraph(graph, file2);
        printf("Node count: %d\n", nodes);
        printGraph(graph);
        fclose(file2);
        int nodeToDelete;
        puts("Enter the node to delete: ");
        scanf("%d", &nodeToDelete);
        deleteNode(graph, nodeToDelete);
        printf("After deleting node %d:\n", nodeToDelete);
        printGraph(graph);
        puts("Sorted graph:");
        sortGraph(graph);
        printGraph(graph);
        saveAsDot(graph);
        freeGraph(graph);

        endTime = clock();
        fprintf(fp, "%d\n", endTime - startTime);
    }
    fclose(fp);
    return 0;



}