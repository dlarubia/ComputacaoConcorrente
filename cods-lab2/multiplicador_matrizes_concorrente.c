#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

float *matA;
float *matB;
float *matC;
int linhasA, linhasB, colunasA, colunasB, nthreads;

/*
 
void *multMatriz (void *tid) {
    int i = * (int *) tid;
    int j, k, produto;

    for (; i < linhasA; i += nthreads) {
        for (j = 0; j < colunasB; j++) {
            produto = 0;
            for (k = 0; k < linhasA; k++)
                produto += matA[i][k + i] * matB[k + i][j];
        }
        matC[i][j] = produto;
    }

    free(tid);
    pthread_exit(NULL);
 }
*/

void *multMatriz (void *tid) {
    int i = * (int *) tid;
    int j, k;
    float acumulador;
    
    printf("\nID DA THREAD: %d\n", i);
    for (; i < colunasA; i+= nthreads) {
        for (j = 0; j < linhasB; j++) {
            acumulador = 0;
            for(k = 0; k < colunasA; k++) {
                acumulador += matA[i * colunasA + k] * matB[k * colunasA + j];
            }
        matC[i*colunasA + j] = acumulador;
        //printf("\nmat[%d]: %f \n", contador, matC[contador]); 
        }
        
    }

/* 
    for (int z = 0; z < colunasA*linhasB ; z++) {
        printf(" -> %f\n", matC[z]);
    }
*/ 
    free(tid);
    pthread_exit(NULL);
 }

 int preencheMatriz(float **mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   //aloca espaco de memoria para a matriz
   *mat = (float*) malloc(sizeof(float) * linhas * colunas);
   if (*mat == NULL) return 0;
   //preenche o vetor
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fscanf(arq, "%f", (*mat) + (i*colunas+j));
      }
   }
   return 1;
}

void imprimeMatriz(float *mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fprintf(arq, "%.1f ", mat[i*colunas+j]);
      }
      fprintf(arq, "\n");
   }
}

int main(int argc, char *argv[]) {
    pthread_t *tid_sistema;
    int *tid;
    FILE *arquivoA, *arquivoB, *arquivoC;
    double inicioApp, fimApp, inicio, fim;

    int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
    printf("\nNúmero de Processadores: %d\n", numCPU);

    GET_TIME(inicioApp);
    GET_TIME(inicio);

    if(argc < 5) {
        fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz B> <arquivo matriz C (saída)> <número de threads (1 a %d)> \n", argv[0], numCPU);
        return 1;
    }

    arquivoA = fopen(argv[1], "r");
    if(arquivoA == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo da matriz de entrada A. \n");
        return 1;
    }

    fscanf(arquivoA, "%d", &linhasA);
    fscanf(arquivoA, "%d", &colunasA);

    arquivoB = fopen(argv[1], "r");
    if(arquivoB == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada B. \n");
        return 1;
    }

    fscanf(arquivoB, "%d", &linhasB);
    fscanf(arquivoB, "%d", &colunasB);

    printf("\nDimensão das matrizes de entrada: %d x %d e %d x %d.\n", linhasA, colunasA, linhasB, colunasB);
    if(colunasA != linhasB) {
        fprintf(stderr, "Erro: as dimensões das matrizes são incompatíveis com a operação.\n");
        return 1;
    }

    arquivoC = fopen(argv[3], "w");
    if(arquivoC == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída C.\n");
        return 1;
    }

    if(atoi(argv[4]) <= numCPU)
        nthreads = atoi(argv[4]);
    else {
        printf("Você não selecionou um número válido de processadores.\n");
    }

    if(preencheMatriz(&matA, linhasA, colunasA, arquivoA) == 0) {
        fprintf(stderr, "Erro ao preencher matriz de entrada A.\n");
        return 1;
    }

    if(preencheMatriz(&matB, linhasB, colunasB, arquivoB) == 0) {
        fprintf(stderr, "Erro ao preencher matriz de entrada B.\n");
        return 1;
    }

    matC = (float*) malloc(sizeof(float) * (colunasA * linhasB));
    if(matC == NULL) {
        fprintf(stderr, "Erro ao alocar a matriz de saída C.\n");
        return 1;
    }

    GET_TIME(fim);
    printf("\nTempo necessário para inicialização do programa: %f \n", fim - inicio);

    GET_TIME(inicio);
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }

    GET_TIME(fim);
    printf("\nTempo para alocação dos identificadores das threads: %f\n", fim - inicio);

    GET_TIME(inicio);
    int t;
    for(t = 0; t < nthreads; t++) {
        tid = malloc(sizeof(int));
        if(tid == NULL) {
            printf("--ERRO: malloc()\n"); 
            exit(-1); 
        }

        *tid = t;
        if(pthread_create(&tid_sistema[t], NULL, multMatriz, (void*) tid)) {
            printf("--ERRO: pthread_create()\n"); 
            exit(-1); 
        }
    }
    GET_TIME(fim);

    printf("\nTempo necessário para criação das threads e multiplicação das matrizes: %f\n", fim - inicio);

    GET_TIME(inicio);
    for(t = 0; t < nthreads; t++) {
        if(pthread_join(tid_sistema[t], NULL)) {
            printf("--ERRO: pthread_join()\n"); 
            exit(-1); 
        }
    }

    GET_TIME(fim);

    printf("Tempo aguardando o encerramento das threads: %f\n", fim - inicio);

    GET_TIME(inicio);

    imprimeMatriz(matC, linhasB, colunasA, arquivoC);
    free(matA);
    free(matB);
    free(matC);

    GET_TIME(fim);

    printf("\nTempo para finalização: %f\n", fim - inicio);

    GET_TIME(fimApp);
    printf("\nTempo de execução total da aplicação: %f \n", fimApp - inicioApp);

    pthread_exit(NULL);

    return 0;
}