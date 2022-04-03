#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var0   = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condition_var1   = PTHREAD_COND_INITIALIZER;

void *functionCount0();
void *functionCount1();
void *functionCount2();

int  count = 0;

#define COUNT_DONE  10
#define COUNT_HALT  3

void main() {
   pthread_t thread0, thread1, thread2;

   pthread_create(&thread0, NULL, &functionCount0, NULL);
   pthread_create(&thread1, NULL, &functionCount1, NULL);
   pthread_create(&thread2, NULL, &functionCount2, NULL);

   pthread_join(thread0, NULL);
   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);

   printf("Final count: %d\n", count);

   exit(EXIT_SUCCESS);
}

void *functionCount0(){
    for(;;){
        pthread_mutex_lock( &count_mutex );
        if(count >= COUNT_DONE){
            pthread_mutex_unlock(&count_mutex);
            pthread_cond_signal(&condition_var0);
            pthread_cond_signal(&condition_var1);
            pthread_exit(NULL);
        }

        int res = count % COUNT_HALT;
        if(res == 0){
            count++;
            printf("Counter value functionCount0: %d\n", count);
            pthread_mutex_unlock(&count_mutex);
        } else if (res == 1) {
            pthread_mutex_unlock(&count_mutex);
            pthread_cond_signal(&condition_var0);
        } else {
            pthread_mutex_unlock(&count_mutex);
            pthread_cond_signal(&condition_var1);
        }
    }
}

void *functionCount1(){
    unsigned long i;
    for(;;){
        pthread_mutex_lock(&count_mutex);

        if(count >= COUNT_DONE){
	        pthread_mutex_unlock( &count_mutex );
            pthread_exit(NULL);
        }
        pthread_cond_wait(&condition_var0, &count_mutex);

        if (count >= COUNT_DONE){
            pthread_mutex_unlock(&count_mutex);
            pthread_exit(NULL);
        }
        count++;
        printf("Counter value functionCount1: %d\n", count);
        pthread_mutex_unlock(&count_mutex);
    }
}

void *functionCount2(){
    unsigned long i;
    for(;;){
        pthread_mutex_lock( &count_mutex );

        if(count >= COUNT_DONE){
	        pthread_mutex_unlock( &count_mutex );
            pthread_exit(NULL);
        }
        pthread_cond_wait(&condition_var1, &count_mutex);

        if (count >= COUNT_DONE){
            pthread_mutex_unlock(&count_mutex);
            pthread_exit(NULL);
        }
        count++;
        printf("Counter value functionCount2: %d\n", count);
        pthread_mutex_unlock(&count_mutex);
    }
}