#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <algorithm>
#include "Process.h"

using namespace std;

// user processes 1-5
void *execUP1(void *);  
void *execUP2(void *);
// i/o processes 1-5
void *execIO1(void *);
void *execIO2(void *);

void updateState(Process, string);

int main() {

    vector<Process> ready;  // ready queue
    queue<Process> waiting;  // waiting queue

    pthread_t up1, up2, io1, io2;

    Process processA("User Process 1", 1, up1, execUP1);
    Process processB("User Process 2", 2, up2, execUP2);
    Process processC("I/O Process 1", 3, io1, execIO1, 3);
    Process processD("I/O Process 2", 4, io2, execIO2, 2);

    ready.push_back(processA);
    ready.push_back(processC);
    ready.push_back(processD);
    ready.push_back(processB);

    // while there are Processes in the ready or waiting queues
    while(!ready.empty() || !waiting.empty()) {
        
        // if there is a Process in ready queue
        if(!ready.empty()) {
            printf("\nSCHEDULER: allocating CPU to %s\n", ready.front().name.c_str());

            pthread_create(&ready.front().tid, NULL, ready.front().func, NULL);  // start running Process at front of ready queue

            // updateState(ready.front(), "RUNNING");

            pthread_join(ready.front().tid, NULL);  // wait for process to finish

            ready.front().cpuBursts[0]++;  // increment number of completed cpu bursts

            if(ready.front().cpuBursts[0] >= ready.front().cpuBursts[1]) {  // if all necessary cpu bursts have completed
                cout << "SCHEDULER: " << ready.front().name << " has completed" << endl;
            }
            else {  // else the process still has input to wait for
                cout << "SCHEDULER: " << ready.front().name << " has blocked for input" << endl;
                //updateState(ready.front(), "BLOCKED");

                waiting.push(ready.front());  // insert at tail of waiting queue
                waiting.front().blockedTime = clock();  // mark the time at which process blocked for input
            }
            ready.erase(ready.begin());  // pop process from head of ready queue
        }
        
        // if there are Processes in waiting queue
        if(!waiting.empty() && ((clock() - waiting.front().blockedTime)/CLOCKS_PER_SEC) > 2) {
            //updateState(waiting.front(), "READY");

            cout << "SCHEDULER: " << waiting.front().name << " has received input" << endl;

            ready.insert(ready.begin(), waiting.front());  // insert process at head of ready queue
            waiting.pop();  // pop process from head of waiting queue
        }

    }

    printf("\nSCHEDULER: all processes finished sucessfully!\n");

    return 0;
}

// user process 1
void *execUP1(void *) {
    system("./userProcess1");
    return NULL;
}

// user process 2
void *execUP2(void *) {
    system("./userProcess2");
    return NULL;
}

// I/O process 1
void *execIO1(void *) {
    system("./ioProcess1");
    return NULL; 
}

// I/O process 2
void *execIO2(void *) {
    system("./ioProcess2");
    return NULL;
}

void updateState(Process p, string newState) {
    string prevState = p.state;
    p.state = newState;
    printf("SCHEDULER: %s has moved from %s state to %s state\n", p.name.c_str(), prevState.c_str(), p.state.c_str());
}
