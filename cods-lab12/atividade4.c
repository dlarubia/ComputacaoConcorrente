#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

int contador = 0, input = 0, output = 0;
pthread_mutex_t mutex;
pthread_cond_t condicao;

void *produz (void *tid) {
    int tid = (int *) tid;
    pthread_mutex_lock(&mutex);
    while(contador == N) {
        printf("Thread %d aguardando para realizar inserção. \n", tid);
        pthread_cond_wait(&condicao, &mutex);
    }
}


void *consome (void *tid) {

}


int main (int argc, char* argv[]) {
    pthread_t tid[6];
    int *id;



    return 0;
}