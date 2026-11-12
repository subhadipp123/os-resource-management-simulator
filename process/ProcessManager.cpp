
#include "ProcessManager.h"

using namespace std;

// Initialize the ProcessManager
ProcessManager::ProcessManager()
    : nextPID(1)
{
}

// Create and store a new process
bool ProcessManager::createProcess(const string& name, int priority)
{
    if (name.empty())
    {
        return false;
    }

    if (priority < 1 || priority > 10)
    {
        return false;
    }

    Process newProcess(
        nextPID,
        name,
        priority,
        ProcessState::New
    );

    processes.push_back(newProcess);

    nextPID++;

    return true;
}

// Return all stored processes
const vector<Process>& ProcessManager::getAllProcesses() const
{
    return processes;
}

// Search for a process using its PID
const Process* ProcessManager::findProcessByPID(int pid) const
{
    for (const Process& process : processes)
    {
        if (process.pid == pid)
        {
            return &process;
        }
    }

    return nullptr;
}

// Change the state of a process
bool ProcessManager::changeProcessState(int pid, ProcessState nextState)
{
    for (Process& process : processes)
    {
        if (process.pid == pid)
        {
            // Termination is handled separately
            if (nextState == ProcessState::Terminated)
            {
                return false;
            }

            if (!isValidTransition(process.state, nextState))
            {
                return false;
            }

            process.state = nextState;

            return true;
        }
    }

    return false;
}

// Terminate a process
bool ProcessManager::terminateProcess(int pid)
{
    for (Process& process : processes)
    {
        if (process.pid == pid)
        {
            if (!isValidTransition(
                    process.state,
                    ProcessState::Terminated))
            {
                return false;
            }

            process.state = ProcessState::Terminated;

            return true;
        }
    }

    return false;
}
