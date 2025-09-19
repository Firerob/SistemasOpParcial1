#include "MLFQ.h"

int main() {
    MLFQ sched;
    sched.readData();

    // Esquema 1
    MLFQ s1 = sched;
    s1.configureScheme(1);
    s1.run();
    s1.writeResults("Esquema 1: RR(1), RR(3), RR(4), SJF");

    // Esquema 2
    MLFQ s2 = sched;
    s2.configureScheme(2);
    s2.run();
    s2.writeResults("Esquema 2: RR(2), RR(3), RR(4), STCF");

    // Esquema 3
    MLFQ s3 = sched;
    s3.configureScheme(3);
    s3.run();
    s3.writeResults("Esquema 3: RR(3), RR(5), RR(6), RR(20)");

    return 0;
}