/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 2 */
/* Codigo: Multiplica uma matriz por um vetor */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include <unistd.h>

//variaveis globais
float *matA; //matriz de entrada
float *matB; //matriz de entrada
float *matC; //matriz de saida
float *vetX; //vetor de entrada
float *vetB; //vetor de saida
int linhas, colunas, nthreads; //dimensoes da matriz de entrada

//funcao que multiplica matriz por vetor (A * X = B)
//entrada: matriz de entrada, vetor de entrada, vetor de saida, dimensoes da matriz
//requisito 1: o numero de colunas da matriz eh igual ao numero de elementos do vetor de entrada
//requisito 2: o numero de linhas da matriz eh igual ao numero de elementos do vetor de saida
void *multiplicaMatrizVetor(void *tid) {
   int i = * (int *) tid;
   int j;
   for (; i<linhas; i += nthreads) {
      vetB[i] = 0;
      for (j = 0; j<colunas; j++) {
         vetB[i] += matA[i*colunas+j] * vetX[j];
      }
   }
   free(tid);
   pthread_exit(NULL);
}

void *multiplicaMatrizMatriz(void *tid) {
   int i = *(int *) tid;
   int j;
   for (; i < linhas; i += nthreads) {
      matC[i] = 0;
      for (j = 0; j < colunas; j ++) {
         matC[i] += matA[i * colunas + j] * matB[j * colunas];
      }
   }
   free(tid);
   pthread_exit(NULL);
}


//funcao que aloca espaco para uma matriz e preenche seus valores
//entrada: matriz de entrada, dimensoes da matriz
//saida: retorna 1 se a matriz foi preenchida com sucesso e 0 caso contrario
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

//funcao que imprime uma matriz
//entrada: matriz de entrada, dimensoes da matriz
//saida: matriz impressa na tela
void imprimeMatriz(float *mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fprintf(arq, "%.1f ", mat[i*colunas+j]);
      }
      fprintf(arq, "\n");
   }
}

//funcao que aloca espaco para um vetor e preenche seus valores
//entrada: vetor de entrada, dimensao do vetor
//saida: retorna 1 se o vetor foi preenchido com sucesso e 0 caso contrario
int preencheVetor(float **vet, int dim, FILE *arq) {
   int i;
   //aloca espaco de memoria para o vetor
   *vet = (float*) malloc(sizeof(float) * dim);
   if (*vet == NULL) return 0;
   //preenche o vetor
   for (i=0; i<dim; i++) {
       //*( (*vet)+i ) = 1.0;
       fscanf(arq, "%f", (*vet) + i);
   }
   return 1;
}

//funcao que imprime um vetor
//entrada: vetor de entrada, dimensao do vetor
//saida: vetor impresso na tela
void imprimeVetor(float *vet, int dim, FILE *arq) {
   int i;
   for (i=0; i<dim; i++) {
      fprintf(arq, "%.1f ", vet[i]);
   }
   fprintf(arq, "\n");
}

//funcao principal
int main(int argc, char *argv[]) {
   pthread_t *tid_sistema;
   int *tid; 
   FILE *arqA, *arqX, *arqB; //arquivos dos dados de entrada e saida
   int dim; //dimensao do vetor de entrada   
   double inicio, fim, delta1, delta2, delta3;
   double inicio_app, fim_app;

   //descobre quantos processadores a maquina possui
  int numCPU = sysconf(_SC_NPROCESSORS_ONLN); 
  printf("Numero de processadores: %d\n", numCPU);

   GET_TIME(inicio_app);
   GET_TIME(inicio);

   //le e valida os parametros de entrada
   //o arquivo da matriz de entrada deve conter na primeira linha as dimensoes da matriz (linha coluna) seguido dos elementos da matriz separados por espaco
   //o arquivo do vetor de entrada deve conter na primeira linha a dimensao do vetor seguido dos elementos separados por espaco
   if(argc < 5) {
      fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo vetor X> <arquivo vetor B> <numero de threads (1 a %d)>.\n", argv[0], numCPU);
      return 1;
   }
   //abre o arquivo da matriz de entrada
   arqA = fopen(argv[1], "r");
   if(arqA == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de entrada.\n");
      return 1;
   }
   //le as dimensoes da matriz de entrada
   fscanf(arqA, "%d", &linhas);
   fscanf(arqA, "%d", &colunas);

   //abre o arquivo do vetor de entrada
   arqX = fopen(argv[2], "r");
   if(arqX == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo do vetor de entrada.\n");
      return 1;;
   }
   //le a dimensao do vetor de entrada
   fscanf(arqX, "%d", &dim);

   //valida as dimensoes da matriz e vetor de entrada
   if(colunas != dim) {
      fprintf(stderr, "Erro: as dimensoes da matriz e do vetor de entrada nao sao compativeis.\n");
      return 1;
   }

   //abre o arquivo do vetor de saida
   arqB = fopen(argv[3], "w");
   if(arqB == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo do vetor de saida.\n");
      return 1;
   }

   if ( atoi(argv[4]) <= numCPU) nthreads = atoi(argv[4]);
   else {
      printf("Você não selecionou um número válido de processadores\n"); 
      return 1;
   }

   //aloca e preenche a matriz de entrada
   if(preencheMatriz(&matA, linhas, colunas, arqA) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de entrada\n");
      return 1;
   }
   //aloca e preenche o vetor de entrada
   if(preencheVetor(&vetX, dim, arqX) == 0) {
      fprintf(stderr, "Erro de preenchimento do vetor de entrada\n");
      return 1;
   }
   //aloca o vetor de saida
   vetB = (float*) malloc(sizeof(float) * linhas);
   if(vetB==NULL) {
      fprintf(stderr, "Erro de alocacao do vetor de saida\n");
      return 1;
   }

    //aloca espaço para o vetor de identificadores das threads do sistema
  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  if(tid_sistema==NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
  }

  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);

   GET_TIME(fim);

   delta1 = fim - inicio;
//---------------------------------------
   /*
   //imprime a matriz de entrada
   printf("Matriz de entrada:\n");
   imprimeMatriz(matA, linhas, colunas, stdout);
   //imprime o vetor de entrada
   printf("Vetor de entrada:\n");
   imprimeVetor(vetX, colunas, stdout);
   */

   GET_TIME(inicio);
   //multiplica a matriz de entrada pelo vetor de entrada
   //multiplicaMatrizVetor(matA, vetX, vetB, linhas, colunas);

   //cria as threads
   int t;
   for(t = 0; t < nthreads; t++) {
      tid = malloc(sizeof(int)); 
      if(tid==NULL) { 
         printf("--ERRO: malloc()\n"); exit(-1); 
      }
      
      *tid = t;
      if (pthread_create(&tid_sistema[t], NULL, multiplicaMatrizVetor, (void*) tid)) {
         printf("--ERRO: pthread_create()\n"); exit(-1);
      }
   }

   //espera todas as threads terminarem e imprime o vetor de saída
   for(t = 0; t < nthreads; t++) {
      if (pthread_join(tid_sistema[t], NULL)) {
         printf("--ERRO: pthread_join()\n"); exit(-1);
      }
   }   

   GET_TIME(fim);

//-------------------------------------
   delta2 = fim - inicio;

   GET_TIME(inicio);

   //salva o vetor de saida no arquivo de saida
   imprimeVetor(vetB, linhas, arqB);

   //libera os espacos de memoria alocados
   free(matA);   
   free(vetX);   
   free(vetB);

   GET_TIME(fim);

   delta3 = fim - inicio;

   GET_TIME(fim_app);

   printf("Tempo de inicialização: %f \nTempo de multiplicação: %f\nTempo de Finalização %f \nTempo de execução total da aplicação: %f \n", delta1, delta2, delta3, fim_app-inicio_app);   
   
   pthread_exit(NULL);
   return 0;
}

