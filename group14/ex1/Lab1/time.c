#include <time.h>
#include <x86intrin.h>
#include <string.h>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sched.h>
 
struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}





int main() {
    /*
    unsigned long long count;
    struct tms now;
    for (int i = 0; i < 10*1000*1000; i++){
        count = times(&now);
    }
    */

    
    struct timespec t1, t2;
    int ns_max = 300;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    
    for(int i = 0; i < 10*1000*1000; i++){
                     
        //struct tms t1, t2;
        
        //unsigned long long t1, t2;
        //t1 = _rdtsc();
        //t2 = _rdtsc();
       
        //int ns = (t2 - t1) / 2.6;
        
        
        clock_gettime(CLOCK_MONOTONIC, &t1);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &t2);
        
        struct timespec diff = timespec_sub(t2, t1);
        long ns = diff.tv_sec * 1000000000L + diff.tv_nsec;
        

       //  times(&t1);
       //  times(&t2);
        
       //  long ns = (t2.tms_utime - t1.tms_utime)*1000000000L/sysconf(_SC_CLK_TCK);
        
        if(ns >= 0 && ns < ns_max){
           histogram[ns]++;
        }
   }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    
    return 0;
}
