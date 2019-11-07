#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

//int n;
int numCPU, nthreads;
long long int n;
float pi;

float calculaPi_sequencial(int n) {
    
    float pi_seq = 0;
    for (int i = 0; i < n; i++) {
        //sinal = pow(-1, i);
        //printf("sinal: %d\ni = %d \n", sinal, i);
        pi_seq += pow(-1, i) * (1.0/(2 * i + 1));
    }
    return 4*pi_seq;

}

/* 
void *calculaPi_1 (void *tid) {
    int i = *(int *) tid;

    for(; i < n; i =+ nthreads) {
        pi += pow(-1, i) * (1.0/(2 * i + 1));
    }

    pi *= 4;

    free(tid);
    pthread_exit(NULL);
}

*/

int main (int argc, char *argv[]) {
    //pthread_t *tid_sistema;
    //int *tid;
    float pi_seq, erro;

    numCPU = sysconf(_SC_NPROCESSORS_ONLN);

    if (argc < 3) {
        printf("./calculaPi.out <numero_de_elementos_da_serie> <numero_de_threads> \n\n");
        return 1;
    }

    if (atoi(argv[2]) > numCPU || atoi(argv[2]) < 1) {
        printf("Informe um número de threads entre 1 e %d\n", numCPU);
        return 1;
    }
    n = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    pi_seq = calculaPi_sequencial(n);
    printf("Valor de PI - Sequencial: %f\n", pi_seq);

    erro = M_PI - pi_seq;
    printf("Erro: %f\n", erro);

    // ------------ Programa Concorrente ---------------- //

    /* 

    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema == NULL) {
        printf("--ERRO: malloc(0)\n");
        exit(-1);
    }
    
    int t;
    for(t = 0; t < nthreads; t++) {
        tid = malloc(sizeof(int));
        if(tid == NULL) {
            printf("--ERRO: malloc(1)\n");
            exit(-1);
        }

        *tid = t;
        if(pthread_create(&tid_sistema[t], NULL, calculaPi_1, (void *) tid)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    for(t = 0; t < nthreads; t++) {
        if(pthread_join(tid_sistema[t], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    printf("Valor de PI - Concorrente: %f\n", pi);
    printf("Erro: %f\n", M_PI - pi);

    */

    return 0;
}