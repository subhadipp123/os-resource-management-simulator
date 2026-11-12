
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <vector>
#include <string>
#include "Process.h"

using namespace std;

class ProcessManager
{
private:
    vector<Process> processes;
    int nextPID;

public:
    ProcessManager();

    bool createProcess(const string& name, int priority);

    const vector<Process>& getAllProcesses() const;

    const Process* findProcessByPID(int pid) const;

    bool changeProcessState(int pid, ProcessState nextState);

    bool terminateProcess(int pid);
};

#endif
