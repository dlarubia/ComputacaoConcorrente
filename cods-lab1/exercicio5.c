/**
 * Exercício 5 - Desafio
 * Objetivo: Implementar um programa concorrente, com duas threads
 * (além da thread principal), para incrementar de 1 cada elemento de um vetor N
 * (10 < N < 100) elementos
 * 
 * Roteiro:
 * 1 -> Comece pensando em como dividir a tarefa de incrementar todos os elementos do vetor entre duas threads
 * 2 -> Qual(is) argumentos deverão ser passados para a função executada pelas threads?
 * 3 -> Na função/thread main faça a inicialização do vetor e imprima seus valores iniciais
 * 4 -> Implemente a tarefa de cada thread
 * 5 -> Na função / thread main imprima os valores finais do vetor
 * 6 -> Teste seu programa
 * 
 * Funções da biblioteca Pthread:
 * a) int pthread_create (pthread_t * thread, const pthread_attr_t *attr, void *(*start_routine) (void *), 
 *  void *arg); --retorna 0 caso tenha sucesso
 * b) void pthread_exit(void *retval); -- retorna o sinal de que a execução foi finalizada
 * c) pthread_join (pthread_t thread, void **retval); -- retorna 0 no caso de sucesso 
 * 
 *  */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

int main() {
    ## Objetivo = Incrementar em 1 cada elemento do vetor N tal que 10 < N < 99
}

