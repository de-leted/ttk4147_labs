#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "scheduling.h"
#include "schedulers.h"

void set_task_state(struct Task *task, enum taskState taskNewState)
{
    pthread_mutex_lock(&taskStateMutex);
    task->state = taskNewState;
    pthread_mutex_unlock(&taskStateMutex);
}

void wait_for_rescheduling(int quantum, struct Task *task)
{
    int startTime;
    int waitTime;

    pthread_mutex_lock(&timeMutex);
    startTime = globalTime;
    pthread_mutex_unlock(&timeMutex);

    do
    {
        pthread_mutex_lock(&timeMutex);
        pthread_cond_wait(&timeCond, &timeMutex);
        waitTime = globalTime - startTime;
        pthread_mutex_unlock(&timeMutex);
    } while (task->state != finished && waitTime < quantum);

    usleep(timeUnitUs / 100);
}

void round_robin(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

// Implement your schedulers here!
void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
        int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(tasks[taskIndex]->totalRuntime+1, tasks[taskIndex]);

        //  Check if the task is finished
        /*if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        } */

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

        int minArrival = 0;
        for (int i = 0; i < taskCount; i++) {
            if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) {
                if (tasks[i]->arrivalTime < tasks[minArrival]->arrivalTime) {
                    minArrival = i;
                    taskIndex = minArrival;
                }
        } 
        }


    } while (globalTime < timeout);
    
}
void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
    {
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval

        wait_for_rescheduling(tasks[taskIndex]->totalRuntime+1, tasks[taskIndex]);

        //  Check if the task is finished
        //if (tasks[taskIndex]->state == finished)
        //{
        //}


        // Find the next task to run
        int minIndex = 0;
        for (int i = 0; i < taskCount; i++) {
            if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) {
                if (tasks[i]->totalRuntime < tasks[minIndex]->totalRuntime) {
                    minIndex = i;
                    taskIndex = minIndex;
                }
        } 
        }

        // if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) 
    } while (globalTime < timeout);
}
}
void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
        {
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval

        wait_for_rescheduling(tasks[taskIndex]->totalRuntime+1, tasks[taskIndex]);

        //  Check if the task is finished
        //if (tasks[taskIndex]->state == finished)
        //{
        //}


        // Find the next task to run
        //int timeWaited = globalTime - tasks[taskIndex]->arrivalTime;
        //int responseRatio = (timeWaited+tasks[taskIndex]->totalRuntime)/tasks[taskIndex]->totalRuntime;
        int timeWaited = 0;
        int highestRatio = 0;
        for (int i = 0; i < taskCount; i++) {
            if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) {
                //timeWaited = globalTime - tasks[i]->arrivalTime;
                if ((((globalTime - tasks[i]->arrivalTime)+tasks[i]->totalRuntime)/tasks[i]->totalRuntime) > highestRatio) {
                    highestRatio = ((globalTime - tasks[i]->arrivalTime)+tasks[i]->totalRuntime)/tasks[i]->totalRuntime;
                    taskIndex = i;
                }
        } 
        }

        // if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) 
    } while (globalTime < timeout);
    }
}
void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval

        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        int minIndex = 0;
        for (int i = 0; i < taskCount; i++) {
            if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) {
                if ((tasks[i]->totalRuntime-tasks[i]->currentRuntime) < (tasks[minIndex]->totalRuntime-tasks[minIndex]->currentRuntime)) {
                    minIndex = i;
                    taskIndex = minIndex;
                }
        } 
        }

        // if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) 
    } while (globalTime < timeout);
}
void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
    int taskIndex = 0;
    int vekt[taskCount];
    
    for (int i = 0; i < taskCount; i++) {
        vekt[i] = 0;
    }

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        if (tasks[taskIndex]->state == preempted){
            vekt[taskIndex] += 1;
        }

        // Find the next task to run
        int nextIndex = 0;
        int prevIndex = 0;
        for (int i = 0; i < taskCount; i++) {
            if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished) {
                prevIndex = i;
                if (vekt[i] < vekt[nextIndex]) {
                    nextIndex = i;
                    taskIndex = nextIndex;
                    /*
                    if (prevIndex == nextIndex) {
                        nextIndex = (taskIndex + 1) % taskCount;
                    }
                    taskIndex = nextIndex;*/
                }
            }
        }
        //taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}
