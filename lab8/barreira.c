#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5
#define PASSOS 5

int threads_paradas = 0;
pthread_mutex_t mutex;
pthread_cond_t condicao_barreira;

typedef struct {
    int contador, id_thread;
} struct_compartilhada;

void barreira(int nthreads) {
    pthread_mutex_lock(&mutex);
    if(threads_paradas < nthreads - 1) {
        threads_paradas++;
        //printf("Threads paradas: %d\n", threads_paradas);
        pthread_cond_wait(&condicao_barreira, &mutex);
    }
    else {
        threads_paradas = 0;
        pthread_cond_broadcast(&condicao_barreira);
    }

    pthread_mutex_unlock(&mutex);

}

void *A (void *arg) {

    int tid = *(int *) arg, i;
    int cont = 0, boba1, boba2;

    for (i = 0; i < PASSOS; i++)  {
        cont ++;
        printf("Thread %d: cont = %d, passo = %d \n", tid, cont, i);
        barreira(NTHREADS);
        boba1 = 100; boba2 = -100; while(boba2 < boba1) boba2++;
    }
    pthread_exit(NULL);
}

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