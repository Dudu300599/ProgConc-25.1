#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define MAX 100000

long long int *buffer;
int N, M, C;
int in = 0, out = 0, count = 0;
int *primos_por_consumidor;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_vazio = PTHREAD_COND_INITIALIZER;

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função da thread produtora
void* produtor(void* arg) {
    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        while (count == M) {
            pthread_cond_wait(&cond_cheio, &mutex);
        }
        buffer[in] = rand() % MAX;
        in = (in + 1) % M;
        count++;
        pthread_cond_signal(&cond_vazio);
        pthread_mutex_unlock(&mutex);
    }

    // Sinaliza para os consumidores que a produção acabou (N valores já foram gerados)
    for (int i = 0; i < C; i++) {
        pthread_mutex_lock(&mutex);
        while (count == M) {
            pthread_cond_wait(&cond_cheio, &mutex);
        }
        buffer[in] = -1; // -1 como sentinela para encerrar consumidor
        in = (in + 1) % M;
        count++;
        pthread_cond_signal(&cond_vazio);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Função da thread consumidora
void* consumidor(void* arg) {
    int id = *(int*)arg;
    int meus_primos = 0;

    while (1) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&cond_vazio, &mutex);
        }

        long long int num = buffer[out];
        out = (out + 1) % M;
        count--;
        pthread_cond_signal(&cond_cheio);
        pthread_mutex_unlock(&mutex);

        if (num == -1) break; // fim

        if (ehPrimo(num)) {
            meus_primos++;
        }
    }

    primos_por_consumidor[id] = meus_primos;
    return NULL;
}

int main() {
    printf("Digite N (quantidade de números), M (tamanho do buffer), C (consumidores): ");
    scanf("%d %d %d", &N, &M, &C);

    buffer = malloc(sizeof(long long int) * M);
    primos_por_consumidor = calloc(C, sizeof(int));
    pthread_t t_produtor, t_consumidores[C];
    int ids[C];

    pthread_create(&t_produtor, NULL, produtor, NULL);

    for (int i = 0; i < C; i++) {
        ids[i] = i;
        pthread_create(&t_consumidores[i], NULL, consumidor, &ids[i]);
    }

    pthread_join(t_produtor, NULL);

    for (int i = 0; i < C; i++) {
        pthread_join(t_consumidores[i], NULL);
    }

    int total_primos = 0, max_primos = 0, vencedora = -1;
    for (int i = 0; i < C; i++) {
        total_primos += primos_por_consumidor[i];
        if (primos_por_consumidor[i] > max_primos) {
            max_primos = primos_por_consumidor[i];
            vencedora = i;
        }
    }

    printf("Total de primos encontrados: %d\n", total_primos);
    printf("Thread consumidora vencedora: %d (com %d primos)\n", vencedora, max_primos);

    free(buffer);
    free(primos_por_consumidor);
    return 0;
}
