#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <semaphore.h>
#include <inttypes.h>

pthread_barrier_t barr;

pthread_mutex_t mutex;

int main(){
        //sem_init(&mutex, 0, 1);
    int nthreads = ;
    pthread_t threadHandle[nthreads];
    pthread_barrier_init(&barr, NULL, nthreads);

    for(int i=0; i<nthreads; i++) {
        pthread_create(&threadHandle[i], NULL, fn, NULL);
    }

    for(int i=0; i<nthreads; i++) {  
        pthread_join(threadHandle[i], NULL);
    }

    pthread_barrier_destroy(&barr);


    //sem_destroy(&mutex);
    return 0;
}