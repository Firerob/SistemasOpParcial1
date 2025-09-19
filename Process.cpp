#include "Process.h"

Process::Process(string id, int bt, int at, int order) {
    pid = id; burst = bt; arrival = at;
    remaining = bt;
    start = end = ct = rt = tat = wt = -1;
    started = finished = false;
    queueId = 1; // TODOS inician en cola 1
    rrQuantumRemaining = 0;
    arrivalOrder = order;
}