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
        int type;
        enum states state;
        void* (*func)(void *);
        Process(string, int, pthread_t, int, states, void* (*)(void*));
        //~Process();

};

Process::Process(string n, int p, pthread_t t, int ty, states s, void* (*f)(void*)) {
    name = n;
    pid = p;
    tid = t;
    type = ty;
    state = s;
    func = f;
}
/*
Process::~Process() {
    delete func;
}
*/
#endif