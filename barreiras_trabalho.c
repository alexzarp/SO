// fiz este com muita ajuda, nunca vi tanto warning nessa vida :/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 100000

pthread_t *thread;
pthread_barrier_t barreira0;
pthread_barrier_t barreira1;

long fatia;
int numThreads, numItera, count, n, s;
double *vet_v, *vet_n, *temp;

void *ThreadCode(void *arg) {
    long _thread = (long) arg;
    printf("Inicializando a Thread de número %ld\n", _thread);

    int inicio = _thread * fatia + 1;
    int fim = inicio + fatia - 1;
    if (fim > n-2) fim = n-2;

    printf("Índice a cada fatia thread %ld, inicio %d, fim %d\n", _thread, inicio, fim);

    for (int i = 0; i < numItera; i++) {
        for(int j = inicio; j < fim; j++) {
            vet_n[j] = (vet_v[j-1] + vet_v[j+1]) /2;
        }
        s = pthread_barrier_wait(&barreira0);
        printf("Está na iteração de número %d e thread de número %ld\n", i, _thread);

        if (s == PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("Última thread: %ld, realizando swap entre os vetores novo e velho\n", _thread);
            temp = vet_n;
            vet_n = vet_v;
            vet_v = temp;
        }
    }


    s = pthread_barrier_wait(&barreira1);
}

int main(int argc, char *argv[]) {
    n = N - 2;
    printf("Digite \"'Número de threads' 'Número de iterações'\"\n");
    scanf("%d %d", &numThreads, &numThreads);
    count = numThreads;

    fatia = ceil(n/count);
    vet_v = calloc(n, sizeof(double));
    vet_n = calloc(n, sizeof(double));
    vet_v[n-1] = 1.0;
    vet_n[n-1] = 1.0;

    thread = calloc(sizeof(pthread_t), numThreads);

    pthread_barrier_init(&barreira0, NULL, numThreads);
    pthread_barrier_init(&barreira1, NULL, numThreads);

    for (long threadNum = 0; threadNum < count; threadNum++) {
        s = pthread_create(&thread[threadNum], NULL, ThreadCode,(void*)threadNum);
        if (s!=0) printf("Error pthread_create");
    }

    for (long threadNum = 0; threadNum < numThreads; threadNum++) {
        s = pthread_join(thread[threadNum], NULL);
        if (s!=0) printf("Error pthread_join");
    }
    printf("\n");

    return 0;
}