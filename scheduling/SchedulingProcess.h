#ifndef SCHEDULING_PROCESS_H
#define SCHEDULING_PROCESS_H

struct SchedulingProcess
{
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
};

struct Result
{
    int pid;
    int arrivalTime;
    int burstTime;
    int startTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
};

#endif
