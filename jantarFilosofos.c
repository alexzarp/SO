#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h> //para função sleep

#define N 5
#define PENSAR 0
#define FOME 1
#define COMER 2
#define ESQUERDA (nfilosofo + 4) % N //agarrar garfo da esquerda
#define DIREITA (nfilosofo + 1) % N  //agarrar garfo da direita

void *filosofo(void *num);
void pegaGarfo(int);
void largaGarfo(int);
void testa(int);

sem_t mutex;
sem_t S[N]; //inicializacao do semaforo
int estado[N];
int nfilosofo[N] = {0, 1, 2, 3, 4};

void *filosofo(void *num) {
    while(1) {
        int *i = num;
        sleep(1);
        pegaGarfo(*i);
        sleep(1);
        largaGarfo(*i);
    }
}

void pegaGarfo(int nfilosofo) {
    sem_wait(&mutex);
    estado[nfilosofo] = FOME;
    printf("Filosofo %d esta com fome.\n", nfilosofo+1);
    //+1 para imprimir filosofo 1 e nao filosofo 0
    testa(nfilosofo);
    sem_post(&mutex);
    sem_wait(&S[nfilosofo]);
    sleep(1);
}

void largaGarfo(int nfilosofo) {
    sem_wait(&mutex);
    estado[nfilosofo] = PENSAR;
    printf("Filosofo %d largou os garfos %d e %d.\n", nfilosofo+1, ESQUERDA+1, nfilosofo+1);
    printf("Filosofo %d esta pensando.\n", nfilosofo+1);
    testa(ESQUERDA);
    testa(DIREITA);
    sem_post(&mutex);
}

void testa(int nfilosofo) {
    if(estado[nfilosofo] == FOME && estado[ESQUERDA] != COMER && estado[DIREITA] != COMER) {
        estado[nfilosofo] = COMER;
        sleep(2);
        printf("Filosofo %d pegou os garfos %d e %d.\n", nfilosofo+1, ESQUERDA+1, nfilosofo+1);
        printf("Filosofo %d esta comendo.\n", nfilosofo+1);
        sem_post(&S[nfilosofo]);
    }
}

int main() {
    int i;
    pthread_t thread_id[N]; //identificadores das threads
    sem_init(&mutex,0,1);

    for(i=0; i<N; i++)
        sem_init(&S[i],0,0);

    for(i=0; i<N; i++) {
        pthread_create(&thread_id[i], NULL, filosofo, &nfilosofo[i]);
        //criar as threads
        printf("Filosofo %d esta pensando.\n",i+1);
    }
    
    for(i=0; i<N; i++)
        pthread_join(thread_id[i],NULL); //para fazer a junção das threads

    return(0);
}