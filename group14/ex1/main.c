#define _GNU_SOURCE

#include <wiringPi.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

void io_init( void ) {
    wiringPiSetup();
    
    pinMode (PININ1, INPUT); pinMode (PINOUT1, OUTPUT); 
    pinMode (PININ2, INPUT); pinMode (PINOUT2, OUTPUT); 
    pinMode (PININ3, INPUT); pinMode (PINOUT3, OUTPUT); 
    
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}


struct responseTaskArgs {
	struct {
		int test;
		int response;
	} pin;
	   // other args ...
};


/*
struct responseTaskArgs args[3] = {
    {{PININ1, PINOUT1}},
    {{PININ2, PINOUT2}},
    {{PININ3, PINOUT3}}
};
*/


/*
static void responseTask(void* args){
	struct responseTaskArgs a = *(struct responseTaskArgs*)args;
	while(1){
		if(gpio_pin_is_high(a.pin.test)){
			//vTaskDelay(1);
			gpio_set_pin_high(a.pin.response);
		}
		
		if(gpio_pin_is_low(a.pin.test)){
			gpio_set_pin_low(a.pin.response);
			//vTaskDelay(0);
		}
	}
}
*/

void sleep_realtime(long nanoseconds)
{
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

int set_cpu(int cpu_id)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return EINVAL;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void *thread_function(void *arg) {
    struct responseTaskArgs *a = (struct responseTaskArgs*)arg;
    set_cpu(1);

    while(1){
        sleep_realtime(2000000);
		if(digitalRead(a->pin.test) == HIGH){
			digitalWrite(a->pin.response, HIGH);
		
        } else {
            digitalWrite(a->pin.response, LOW);
        }
    }
    return NULL;
}

void *disturbance(void *arg) {
    set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    }
}


int main() {
    io_init();


    pthread_t threads[3];
    pthread_t disturb[10];


    struct responseTaskArgs args[3] = {
    {{PININ1, PINOUT1}},
    {{PININ2, PINOUT2}},
    {{PININ3, PINOUT3}}
    };


    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, &args[i]);
    }

    /*
    for (int i = 0; i < 10; i++) {
        pthread_create(&disturb[i], NULL, disturbance, NULL);
    }
    */

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    /*
    for (int i = 0; i < 10; i++) {
        pthread_join(disturb[i], NULL);
    }
    */

    /*
    xTaskCreate(responseTask, "", 1024, (&(struct responseTaskArgs){{PININ1, PINOUT1}, NULL}), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(responseTask, "", 1024, (&(struct responseTaskArgs){{PININ2, }, NULL}), tskIDLE_PRIORITY + 1, NULL);
	//xTaskCreate(responseTask_C, "", 1024, (&(struct responseTaskArgs){{TEST_C, RESPONSE_C}, NULL}), tskIDLE_PRIORITY + 1, NULL);
	
	vTaskStartScheduler();
    */


    return 0;
}