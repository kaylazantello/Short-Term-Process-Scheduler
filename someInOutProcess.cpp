#include <iostream>
#include <unistd.h>

using namespace std;

int main() {

    printf("\nI am an I/O process\n");

    for(int i = 0; i < 10; i++) {
        printf("Accessing the disk...\n");

        sleep(2);
    }


    return 0;
}