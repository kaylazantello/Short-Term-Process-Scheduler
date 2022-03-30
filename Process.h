#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <algorithm>

using namespace std;

enum states {
    RUNNING, READY, BLOCKED, IDLE
};

class Process {
    public:
        string name;
        int pid;
        pthread_t tid;
        string type;
        string state;
        void* (*func)(void *);
        int cpuBursts[2];
        int blockedTime;
        Process(string, int, pthread_t, void* (*)(void*));
        Process(string, int, pthread_t, void* (*)(void*), int);
        //~Process();

};

// constructor for a user process
Process::Process(string n, int p, pthread_t t, void* (*f)(void*)) {
    name = n;
    pid = p;
    tid = t;
    type = "user";
    state = "READY";
    func = f;
    cpuBursts[0] = 0;  // has had 0 cpu bursts at initialization
    cpuBursts[1] = 1;  // only requires 1 cpu burst to complete
    blockedTime = 0;
}

// constructor for i/o process
Process::Process(string n, int p, pthread_t t, void* (*f)(void*), int c) {
    name = n;
    pid = p;
    tid = t;
    type = "I/O";
    state = "READY";
    func = f;
    cpuBursts[0] = 0;  // has had 0 cpu bursts at initialization
    cpuBursts[1] = c;  // number of cpu bursts required to complete
    blockedTime = 0;
}
/*
Process::~Process() {
    delete func;
}
*/
#endif