#include <iostream>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <cstdlib>

using namespace std;

int n = 10;  // size of buffer
mutex mtx;  // mutex to control access to buffer
int full = 0;  // number of occupied spots in buffer
int empty = n;  // number of open spots in buffer
vector<int> buffer;  // the shared buffer
int numProduced;  // keep track of the number of items produced
int numConsumed;  // keept track of the number of items consumed

// semaphore operation - decrease by 1
void wait(int *semaphore) {
    *semaphore -= 1;
}

// semaphore operation - increase by 1
void signal(int *semaphore) {
    *semaphore += 1;
}

// producer function
void producer() { 
    
    mtx.lock();  // lock mutex to keep consumer from accessing buffer

    // if there are empty spaces in buffer
    if(empty > 0) {

        int x = rand() % empty + 1;  // random number of items to be produced (between 1 and number of empty spots)

        for(int i = 0; i < x; i++) {
            buffer.push_back(i);  // insert into buffer
            wait(&empty);  // decrease number of empty spots
            signal(&full);  // increase number of full spots
            numProduced++;  // increment numProduced variable
        }

        cout << "P: produced " << x << " items" << endl;
        mtx.unlock();  // unlock mutex so that consumer may have access to buffer
        return;
    }
    // else the buffer is full
    else {
        mtx.unlock();  // unlock mutex so consumer may have access to buffer
        return;
    }
}

// consumer function
void consumer() {
   
    mtx.lock();  // lock the mutex to keep producer from accessing buffer

    // if there are occupied spots in buffer
    if(full > 0) {

        int x = rand() % full + 1;  // random number of items to be consumed (between 1 and number of occupied spots)

        for(int i = 0; i < x; i++) {
            buffer.pop_back();  // remove item from buffer
            wait(&full);  // decrease number of full spots
            signal(&empty);  // increase number of empty spots
            numConsumed++;  // increment numConsumed variable
        }

        cout << "C: consumed " << x << " items" << endl;
        mtx.unlock();  // unlock mutex so that consumer may have access

        return;
    }
    // else the buffer is empty
    else {
        mtx.unlock();  // unlock mutex so producer may have access to buffer
        return;
    }
}

int main() {

    srand(time(0));  // ensure different random results each run by using time as seed

    while(1) {

        producer();
        consumer();

        // if producer has produced more than 15 items
        if(numProduced > 15) {
            // allow consumer to run until all items have been consumed
            while(1) {
                consumer();

                if(buffer.empty()) {
                    cout << "\nTOTAL # OF ITEMS PRODUCED: " << numProduced << endl;
                    cout << "TOTAL # OF ITEMS CONSUMED: " << numConsumed << endl;
                    return 0;
                }
            }
        }
    }

    return 0;
}