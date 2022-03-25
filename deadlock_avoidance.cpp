/* 
This code was created by Kayla, Ethan, and Kara
 The purpose of this code is to create a system that avoids deadlocks
 The program does this by having a wait variable that when a thread
 waits more than 2 seconds for more resources to become available it 
 releases its resources so that another thread may complete its process
 so that the original thread now has enough resources.
*/
#include<iostream>
#include<pthread.h>
#include <unistd.h>

using namespace std;

int available = 2; //number of available resources

bool request(int num, int *avail) { //process is requesting a resource
    if(num <= *avail) {
        *avail -= num;
        return true; //if there are available resources return true
    }

    else{
        return false; //else return false
    }
}

void returnR(int num, int *avail) { //process will return all used resources
    *avail += num;
}

void *processA(void *) {

    printf("Process A: started\n");
    clock_t timerA;
    int countA = 1; // A starts with 1 resource
    int maxA = 4; //A needs 4 resources
    bool waitingA = false;

    while(1) {

        if(request(1, &available)) { //if able to aquire a resource increase count towards finishing process
            waitingA = false;
            countA++;
            printf("Process A: acquired %d of %d\n", countA, maxA);
        }
        else if(!waitingA) { //if A needs to wait on a resource start the waiting process
            waitingA = true;
            timerA = clock() / CLOCKS_PER_SEC;
        }

        if(countA == maxA) { //once process A has enough resources the process is completed and the resources are returned for other processes to use
            printf("Process A: completed. returning %d resources\n", countA);
            returnR(countA, &available);
            return NULL; //get out of while loop
        }

	    // if the process has to wait more than 2 seconds the process quits and returns all resources it had
        if((((clock() / CLOCKS_PER_SEC) - timerA) > 2) && waitingA) {  
            returnR(countA, &available);
            printf("Process A: waited too long, returned %d resources\n", countA);
            countA = 0; //process now has zero resources
            sleep(2);
        }
    }
}

void *processB(void *) {

    printf("Process B: started\n");
    clock_t timerB;
    int countB = 1; //B starts with one resource
    int maxB = 5; //B needs five resources to complete
    bool waitingB = false;

    while(1) {

        if(request(1, &available)) { //if able to aquire a resource increase count towards finishing process
            waitingB = false;
            countB++;
            printf("Process B: acquired %d of %d\n", countB, maxB);
        }
        else if(!waitingB) { //if B needs to wait on a resource start the waiting process
            waitingB = true;
            timerB = clock() / CLOCKS_PER_SEC;
        }

        if(countB == maxB) { //once process B has enough resources the process is completed and the resources are returned for other processes to use
            printf("Process B: completed. returning %d resources\n", countB);
            returnR(countB, &available);
            return NULL; //get out of while loop
        }

	    // if the process has to wait more than 2 seconds the process quits and returns all resources it had
        if((((clock() / CLOCKS_PER_SEC) - timerB) > 2) && waitingB) {
            returnR(countB, &available);
            printf("Process B: waited too long, returned %d resources\n", countB);
            countB = 0; //process now has zero resources
            sleep(2);
        }
    }
}



void *processC(void *) {

    printf("Process C: started\n");
    clock_t timerC;
    int countC = 1; //C starts with 1 resource
    int maxC = 4; //C needs four resources to complete the process
    bool waitingC = false;

    while(1) {

        if(request(1, &available)) { //if able to aquire a resource increase count towards finishing process
            waitingC = false;
            countC++;
            printf("Process C: acquired %d of %d\n", countC, maxC);
        }

        else if(!waitingC) { //if C needs to wait on a resource start the waiting process
            waitingC = true;
            timerC = clock() / CLOCKS_PER_SEC;
        }

        if(countC == maxC) { //once process C has enough resources the process is completed and the resources are returned for other processes to use
            printf("Process C: completed. returning %d resources\n", countC);
            returnR(countC, &available);
            return NULL; //exit while loop
        }

	    // if the process has to wait more than 2 seconds the process quits and returns all resources it had
        if((((clock() / CLOCKS_PER_SEC) - timerC) > .1) && waitingC) {
            returnR(countC, &available);
            printf("Process C: waited too long, returned %d resources\n", countC);
            countC = 0; //process now has zero resources
            sleep(2);
        }
    }
}

int main() 
{
    // show resource summary
    cout << "Process:                      A  B  C" << endl;
    cout << "Maximum resource needs:       4  5  4" << endl;
    cout << "Current resource allocation:  1  1  1" << endl;
    cout << "Current resource needs:       3  4  3" << endl;

    cout << "\nResources available: 2\n" << endl;

    pthread_t pA, pB, pC;

    sleep(5);

    // creating the threads and running the three different processes
    pthread_create(&pA, NULL, processA, NULL);
    pthread_create(&pB, NULL, processB, NULL);
    pthread_create(&pC, NULL, processC, NULL);

    // wait for all threads to terminate before exiting program
    pthread_join(pA, NULL);
    pthread_join(pB, NULL);
    pthread_join(pC, NULL);

    cout << "\nAll processes have successfully completed!" << endl;

    return 0;

}

