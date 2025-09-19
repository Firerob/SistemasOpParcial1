#include "MLFQ.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Configura los esquemas de planificación según lo definido:
// Cada esquema determina qué algoritmo se usa en cada cola y el quantum correspondiente
void MLFQ::configureScheme(int scheme) {
    if (scheme == 1) {
        qAlgo[1] = RR_TYPE; qQuantum[1] = 1;
        qAlgo[2] = RR_TYPE; qQuantum[2] = 3;
        qAlgo[3] = RR_TYPE; qQuantum[3] = 4;
        qAlgo[4] = SJF_TYPE; qQuantum[4] = 0;
    } else if (scheme == 2) {
        qAlgo[1] = RR_TYPE; qQuantum[1] = 2;
        qAlgo[2] = RR_TYPE; qQuantum[2] = 3;
        qAlgo[3] = RR_TYPE; qQuantum[3] = 4;
        qAlgo[4] = STCF_TYPE; qQuantum[4] = 0;
    } else if (scheme == 3) {
        qAlgo[1] = RR_TYPE; qQuantum[1] = 3;
        qAlgo[2] = RR_TYPE; qQuantum[2] = 5;
        qAlgo[3] = RR_TYPE; qQuantum[3] = 6;
        qAlgo[4] = RR_TYPE; qQuantum[4] = 20;
    }
}

// Leer datos de entrada desde stdin (se espera formato PID;BT;AT;...)
// Cada proceso se guarda con un orden de llegada (order)
void MLFQ::readData() {
    string line; int order = 0;
    while (getline(cin, line)) {
        if (line.empty() || line[0] == '#') continue; // ignorar líneas vacías o comentarios
        vector<string> tokens; 
        string token; 
        stringstream ss(line);

        while (getline(ss, token, ';')) tokens.push_back(token);

        if (tokens.size() < 3) continue; // aseguramos que tenga al menos PID, BT y AT
        string pid = tokens[0];
        int bt = stoi(tokens[1]);
        int at = stoi(tokens[2]);
        procs.push_back(Process(pid, bt, at, order++));
    }
}

// Ejecutar la planificación MLFQ
void MLFQ::run() {
    int n = procs.size();
    int time = 0, finishedCount = 0;

    // Mientras no se terminen todos los procesos
    while (finishedCount < n) {
        vector<int> ready[5];

        // Identificar procesos listos en cada cola
        for (int i = 0; i < n; i++) {
            if (!procs[i].finished && procs[i].arrival <= time) {
                ready[procs[i].queueId].push_back(i);
            }
        }
        // Seleccionar la cola más prioritaria (menor número) con procesos listos
        int qid = 0;
        for (int q = 1; q <= 4; q++) {
            if (!ready[q].empty()) { qid = q; break; }
        }

        // Si no hay procesos listos, avanzar el tiempo
        if (qid == 0) { time++; continue; }
        
        // Selección de proceso según el algoritmo de la cola
        int idx = -1;
        if (qAlgo[qid] == RR_TYPE) {
            idx = ready[qid][0];
            if (procs[idx].rrQuantumRemaining == 0)
                procs[idx].rrQuantumRemaining = qQuantum[qid];
        }
        else if (qAlgo[qid] == SJF_TYPE) {
            int bestBurst = 1e9;
            for (int i : ready[qid]) {
                if (procs[i].burst < bestBurst) {
                    bestBurst = procs[i].burst;
                    idx = i;
                }
            }
        }
        else if (qAlgo[qid] == STCF_TYPE) {
            int bestRem = 1e9;
            for (int i : ready[qid]) {
                if (procs[i].remaining < bestRem) {
                    bestRem = procs[i].remaining;
                    idx = i;
                }
            }
        }
        // Referencia al proceso seleccionado
        Process &p = procs[idx];
        
        // Si es la primera vez que corre, calculamos su RT (Response Time)
        if (!p.started) {
            p.start = time;
            p.rt = time - p.arrival;
            p.started = true;
        }
        
        // Ejecutamos una unidad de tiempo
        p.remaining--;
        time++;

        // Decrementar quantum si es Round Robin
        if (qAlgo[qid] == RR_TYPE) p.rrQuantumRemaining--;

        // Si el proceso terminó
        if (p.remaining == 0) {
            p.finished = true;
            p.end = time;
            p.ct = p.end;
            p.tat = p.ct - p.arrival;
            p.wt = p.tat - p.burst;
            finishedCount++;
        } else {
            // Si se le acabó el quantum en RR, baja de cola
            if (qAlgo[qid] == RR_TYPE && p.rrQuantumRemaining == 0) {
                if (p.queueId < 4) p.queueId++;
                p.rrQuantumRemaining = 0;
            }
        }
    }
}

void MLFQ::writeResults(string titulo) {
    cout << "==== " << titulo << " ====\n";
    cout << "PID;BT;AT;Q;WT;CT;RT;TAT\n";
    double sumWT=0,sumCT=0,sumRT=0,sumTAT=0;
    int n = procs.size();
    
    for (auto &p : procs) {
        cout << p.pid << ";" << p.burst << ";" << p.arrival << ";"
             << p.queueId << ";" << p.wt << ";" << p.ct << ";"
             << p.rt << ";" << p.tat << "\n";
        sumWT += p.wt; sumCT += p.ct; sumRT += p.rt; sumTAT += p.tat;
    }
    cout << "WT=" << fixed << setprecision(2) << sumWT/n
         << "; CT=" << sumCT/n
         << "; RT=" << sumRT/n
         << "; TAT=" << sumTAT/n << "\n\n";
}