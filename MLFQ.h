#ifndef MLFQ_H
#define MLFQ_H

#include <vector>
#include <string>
#include "Process.h"
using namespace std;

enum SchedulerType { RR_TYPE, SJF_TYPE, STCF_TYPE };

struct MLFQ {
    vector<Process> procs;
    int qQuantum[5];
    SchedulerType qAlgo[5];

    void configureScheme(int scheme);
    void readData();
    void run();
    void writeResults(string titulo);
};

#endif