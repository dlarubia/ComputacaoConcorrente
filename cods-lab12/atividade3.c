#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t mutex;
int aux;

void *func1 (void *tid) {
    printf("Olá, tudo bem?\n");
    sem_wait(&mutex);
    aux++;
    sem_post(&mutex);
}


void *func2 (void *tid) {
    printf("Hello!\n");
    sem_wait(&mutex);
    aux++;
    sem_post(&mutex);
}


void *func3 (void *tid) {
    while(aux < 2) {
        
    }
    printf("Até mais tarde!\n");
}


void *func4 (void *tid) {
    while(aux < 2) {

    }
    printf("Tchau!\n");
}


int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int *id;

    sem_init(&mutex, 0, 1);

    /*
    for(int i = 0; i < NTHREADS; i++) {
        if(id = malloc(sizeof(int)) == NULL) {
            pthread_exit(NULL);
            return 1;
        }

        *id = i;
    */
    if(pthread_create(&tid[0], NULL, func1, (void *) id)) {
        printf("--ERRO: pthread_create()\n"); 
        exit(-1);
    }

    if(pthread_create(&tid[1], NULL, func2, (void *) id)) {
        printf("--ERRO: pthread_create()\n"); 
        exit(-1);
    }
    if(pthread_create(&tid[2], NULL, func3, (void *) id)) {
        printf("--ERRO: pthread_create()\n"); 
        exit(-1);
    }
    if(pthread_create(&tid[3], NULL, func4, (void *) id)) {
        printf("--ERRO: pthread_create()\n"); 
        exit(-1);
    }

    for(int i = 0; i < NTHREADS; i++) {
        if(pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        }
    }
}