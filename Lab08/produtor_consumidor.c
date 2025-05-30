#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

//Função de primalidade
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (long long int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

//Variáveis Globais
int *buffer;
int M, N, C; // Tamanho do buffer, qtd de números, qtd de consumidores
int in = 0, out = 0, count = 0;
int fim_producao = 0;

int *primos_por_consumidor;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_vazio = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cheio = PTHREAD_COND_INITIALIZER;

//Produtor
void *produtora(void *arg) {
    for (int i = 1; i <= N; i++) {
        pthread_mutex_lock(&mutex);

        while (count == M)
            pthread_cond_wait(&cond_cheio, &mutex);

        buffer[in] = i;
        in = (in + 1) % M;
        count++;

        pthread_cond_signal(&cond_vazio);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    fim_producao = 1;
    pthread_cond_broadcast(&cond_vazio); //acorda todos os consumidores
    pthread_mutex_unlock(&mutex);

    return NULL;
}

//Consumidor
void *consumidora(void *arg) {
    int id = *(int *)arg;
    free(arg);
    int local_primos = 0;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && !fim_producao)
            pthread_cond_wait(&cond_vazio, &mutex);

        if (count == 0 && fim_producao) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        int numero = buffer[out];
        out = (out + 1) % M;
        count--;

        pthread_cond_signal(&cond_cheio);
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(numero)) {
            local_primos++;
        }
    }

    primos_por_consumidor[id] = local_primos;
    return NULL;
}

//Main
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <N> <M> <C>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);  // Quantidade de números a produzir
    M = atoi(argv[2]);  // Tamanho do buffer
    C = atoi(argv[3]);  // Quantidade de threads consumidoras

    buffer = (int *)malloc(sizeof(int) * M);
    primos_por_consumidor = (int *)calloc(C, sizeof(int));

    pthread_t tid_produtora;
    pthread_t consumidores[C];

    pthread_create(&tid_produtora, NULL, produtora, NULL);

    for (int i = 0; i < C; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&consumidores[i], NULL, consumidora, id);
    }

    pthread_join(tid_produtora, NULL);
    for (int i = 0; i < C; i++) {
        pthread_join(consumidores[i], NULL);
    }

    int total = 0;
    int vencedor = 0;
    for (int i = 0; i < C; i++) {
        total += primos_por_consumidor[i];
        if (primos_por_consumidor[i] > primos_por_consumidor[vencedor]) {
            vencedor = i;
        }
    }

    printf("\nTotal de primos encontrados: %d\n", total);
    printf("Thread consumidora vencedora: %d (com %d primos)\n", vencedor, primos_por_consumidor[vencedor]);

    free(buffer);
    free(primos_por_consumidor);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_vazio);
    pthread_cond_destroy(&cond_cheio);

    return 0;
}
