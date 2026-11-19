#ifndef SCHEDULING_PROCESS_H
#define SCHEDULING_PROCESS_H

struct SchedulingProcess
{
    int pid;
    int arrivalTime;
    int burstTime;
};

struct FCFSResult
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