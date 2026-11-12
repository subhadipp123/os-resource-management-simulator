
#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

enum class ProcessState
{
    New,
    Ready,
    Running,
    Waiting,
    Terminated
};

struct Process
{
    int pid;
    string name;
    int priority;
    ProcessState state;

    Process();

    Process(int id, string processName, int p, ProcessState s);
};

string getStateName(ProcessState state);

bool isValidTransition(ProcessState current, ProcessState next);

#endif
