#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using namespace std;

struct Process {
    string pid;
    int burst, arrival, remaining;
    int start, end, ct, rt, tat, wt;
    bool started, finished;
    int queueId;
    int rrQuantumRemaining;
    int arrivalOrder;

    Process(string id, int bt, int at, int order);
};

#endif