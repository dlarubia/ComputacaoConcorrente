#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5;

pthread_mutex_t mutex;
pthread_cond_t condicao_barreira;

typedef struct {
    int contador, id_thread;
} struct_compartilhada;

int main (int argc, int *argv[]) {
    pthread_t tid_sistema[NTHREADS];
    int t, id[NTHREADS];

    for (t = 0; t < NTHREADS; t++) {
        id[t] = t;
        if(pthread_create(&tid_sistema[t], NULL, A, (void *) &id[t])) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (t=0; t < NTHREADS; t++) {
        if (pthread_join(tid_sistema[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    return 0;
}