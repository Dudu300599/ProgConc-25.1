#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

// Função fornecida para testar primalidade
int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

// Variáveis globais compartilhadas
long long int N;
long long int atual = 1; // Número atual para testar
int totalPrimos = 0; // Contador de números primos
pthread_mutex_t mutexAtual; // Mutex para proteger acesso ao 'atual'
pthread_mutex_t mutexContador; // Mutex para proteger 'totalPrimos'

// Função que cada thread vai executar
void *tarefa(void *arg) {
    long long int num;

    while (1) {
        // Pegar o próximo número para testar
        pthread_mutex_lock(&mutexAtual);
        if (atual > N) {
            pthread_mutex_unlock(&mutexAtual);
            break;
        }
        num = atual;
        atual++;
        pthread_mutex_unlock(&mutexAtual);

        // Testar se é primo
        if (ehPrimo(num)) {
            pthread_mutex_lock(&mutexContador);
            totalPrimos++;
            pthread_mutex_unlock(&mutexContador);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <nThreads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    int nThreads = atoi(argv[2]);
    pthread_t threads[nThreads];

    double start, finish, elapsed;

    // Inicializar os mutexes
    pthread_mutex_init(&mutexAtual, NULL);
    pthread_mutex_init(&mutexContador, NULL);

    GET_TIME(start);

    // Criar threads
    for (int i = 0; i < nThreads; i++) {
        pthread_create(&threads[i], NULL, tarefa, NULL);
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    GET_TIME(finish);
    elapsed = finish - start;


    printf("Total de primos entre 1 e %lld: %d\n", N, totalPrimos);
    printf("Tempo de execucao com %d threads: %lf segundos\n", nThreads, elapsed);

    // Destruir mutexes
    pthread_mutex_destroy(&mutexAtual);
    pthread_mutex_destroy(&mutexContador);

    return 0;
}
