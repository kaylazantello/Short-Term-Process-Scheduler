#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <algorithm>
#include "Process.h"

using namespace std;

int timeQuantum = 2;

// user process 1
void *execA(void *) {
    printf("\nProcess A: starting...\n");

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);  // allow thread to be cancelled at any point in time

    string userProcessExecutable1 = "./userProcess";
    system(userProcessExecutable1.c_str());

    printf("Process A: finished\n");
    return NULL;
}

// user process 2
void *execB(void *) {
    printf("\nProcess B: starting...\n");

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);  // allow thread to be cancelled at any point in time

    string userProcessExecutable2 = "./semaphore";
    system(userProcessExecutable2.c_str());

    printf("Process B: finished\n");
    return NULL;
}

// I/O process 3
void *execC(void *) {
    printf("\nProcess C: starting...\n");

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);  // allow thread to be cancelled at any point in time

    string ioProcessExecutable1 = "./inOut";
    //system(ioProcessExecutable1.c_str());

    printf("I am an I/O process\n");

    for(int i = 0; i < 10; i++) {
        printf("Accessing the disk...\n");

        sleep(2);
    }

    printf("Process C: finished\n");
    return NULL; 
}

// I/O process 4
void *execD(void *) {
    printf("Process D: starting...\n");

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);  // allow thread to be cancelled at any point in time

    string ioProcessExecutable2 = "./deadlock";
    system(ioProcessExecutable2.c_str());

    printf("Process D: finished\n");
    return NULL;
}

int main() {

    srand(time(0));  // ensure a random number each run

    vector<Process> ready;  // ready queue
    queue<Process> waiting;  // waiting queue

    pthread_t pA, pB, pC, pD;

    Process processA("Process A", 1, pA, 1, READY, execA);
    Process processB("Process B", 1, pB, 1, READY, execB);
    Process processC("Process C", 1, pC, 2, READY, execC);

    ready.push_back(processA);
    ready.push_back(processC);
    ready.push_back(processB);

/*
    // used for testing pthread_cancel and getting cancellation status after pthread_join

    printf("Scheduler: starting %s...\n", ready.front().name.c_str());
    pthread_create(&ready.front().tid, NULL, ready.front().func, NULL);

    sleep(timeQuantum);
    printf("Finished sleep\n");

    printf("Trying to cancel %s...\n", ready.front().name.c_str());
    int c = pthread_cancel(ready.front().tid);
    if(c == 0) {
        printf("cancellation request successfully queued\n");
    }
    else {
        printf("error with cancellation request\n");
    }

    void *res;
    int j = pthread_join(ready.front().tid, &res);
    if(j == 0) {
        printf("pthread joined successfully\n");
    }
    else {
        printf("error with pthread join\n");
    }

    if(res == PTHREAD_CANCELED) {
        printf("pthread cancelled successfully\n");
    }
    else {
        printf("error cancelling pthread\n");
    }
*/

    // while there are Processes in the ready or waiting queues
    while(!ready.empty() || !waiting.empty()) {
        
        // if there is a Process in ready queue
        if(!ready.empty()) {
            printf("\nSCHEDULER: starting %s then waiting %d...\n", ready.front().name.c_str(), timeQuantum);

            pthread_create(&ready.front().tid, NULL, ready.front().func, NULL);  // start running Process at front of ready queue

            ready.front().state = RUNNING;  // update process state
            printf("SCHEDULER: %s has moved from READY state to RUNNING state\n", ready.front().name.c_str());

            sleep(timeQuantum);  // wait for length of time quantum

            // if there are no other processes in ready queue and current process is I/O
            if(ready.size() == 1 && ready.front().type == 2) {
                sleep(20);  // give process more time to finish
            }

            // try to terminate the thread
            if (pthread_cancel(ready.front().tid) != 0) { // if cancellation request throws error, then thread had already terminated
                printf("\nSCHEDULER: %s has moved from RUNNING state to IDLE state...\n", ready.front().name.c_str());
                ready.front().state = IDLE;  // update process state

                printf("SCHEDULER: removing %s from ready queue...\n", ready.front().name.c_str());
                ready.erase(ready.begin()); // pop thread from head of ready queue
            }
            else { // else if request is successful, thread was not finished and therefore took too long
                printf("\nSCHEDULER: %s has moved from RUNNING state to WAITING state\n", ready.front().name.c_str());
                ready.front().state = BLOCKED;  // update process state

                printf("SCHEDULER: moving %s to waiting queue...\n", ready.front().name.c_str());
                waiting.push(ready.front()); // move thread to tail of waiting queue

                printf("SCHEDULER: removing %s from ready queue...\n", ready.front().name.c_str());
                ready.erase(ready.begin()); // pop thread from head of ready queue
            }
        }
        
        // if there are Processes in waiting queue
        if(!waiting.empty()) {
            // randomly decide if process at head of waiting queue switches to ready state
            if((rand() % 100 + 1) <= 75) {
                printf("\nSCHEDULER: %s has moved from WAITING state to READY state\n", waiting.front().name.c_str());
                waiting.front().state = READY;  // update process state

                printf("SCHEDULER: moving %s to front of ready queue...\n", waiting.front().name.c_str());
                ready.insert(ready.begin(), waiting.front());  // insert process at head of ready queue
                
                printf("SCHEDULER: removing %s from waiting queue...\n", waiting.front().name.c_str());
                waiting.pop();  // pop process from head of waiting queue
            }
        }

    }

    printf("\nSCHEDULER: all processes finished sucessfully!\n");

    return 0;
}