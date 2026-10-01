#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <semaphore.h>
#include <inttypes.h>

#define N 5

pthread_barrier_t barr;
pthread_mutex_t forks[N];

void* philosopher(void* args){
    int phil_ID = *(int *)args;
    int left = phil_ID;
    int right = (phil_ID+1) % N;

    pthread_mutex_lock(&forks[left]);
    printf("Philsopher picked up fork \n");

    pthread_barrier_wait(&barr);

    pthread_mutex_lock(&forks[right]);
    printf("Philsopher is eating \n");

    pthread_mutex_unlock(&forks[right]);
    pthread_mutex_unlock(&forks[left]);
    return NULL;

}

int main(){
    int nthreads = 5;
    int id[nthreads];

    pthread_t threadHandle[nthreads];
    pthread_barrier_init(&barr, NULL, nthreads);

    for(int i=0; i<nthreads; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }    

    for(int i=0; i<nthreads; i++) {
        id[i] = i;
        pthread_create(&threadHandle[i], NULL, philosopher, &id[i]);
    }

    for(int i=0; i<nthreads; i++) {  
        pthread_join(threadHandle[i], NULL);
    }

    for(int i=0; i<nthreads; i++) {
        pthread_mutex_destroy(&forks[i]);
    } 

    pthread_barrier_destroy(&barr);

    return 0;
}