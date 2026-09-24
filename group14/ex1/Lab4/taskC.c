    #define _XOPEN_SOURCE 600
    #include <pthread.h>
    #include <stdio.h>
    #include <semaphore.h>


    pthread_barrier_t barr;

    sem_t mutex;

    long global = 0;

    // Note the argument and return types: void*
    void* fn(void* args){
        
        sem_wait(&mutex);
        //printf("\nJust exiting...\n");

        long local = 0; 
        for (int i = 0; i < 1000000; i++) {
            local++;
            global++;
        }
        sem_post(&mutex);
        
        printf("%ld\n", local);
        printf("%ld\n", global);

        int res = pthread_barrier_wait(&barr);
        if(res == PTHREAD_BARRIER_SERIAL_THREAD) {
        // this is the unique "serial thread"; you can e.g. combine some results here
        } else if(res != 0) {
        // error occurred
        } else {
        // non-serial thread released
        }

        
        return NULL;
    }

    int main(){
        sem_init(&mutex, 0, 1);
        int nthreads = 2;
        pthread_t threadHandle[nthreads];
        pthread_barrier_init(&barr, NULL, nthreads);

        for(int i=0; i<nthreads; i++) {
            pthread_create(&threadHandle[i], NULL, fn, NULL);
        }

        for(int i=0; i<nthreads; i++) {  
            pthread_join(threadHandle[i], NULL);
        }

        pthread_barrier_destroy(&barr);


        sem_destroy(&mutex);
        return 0;
    }