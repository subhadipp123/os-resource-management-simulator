
#include <iostream>
#include <string>
#include <vector>

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

    Process()
        : pid(0), name("Unknown"),
          priority(0), state(ProcessState::New) //default
    {
    }

    Process(int id, string processName, int p, ProcessState s)
        : pid(id), name(processName),
          priority(p), state(s) // initializer list
    {
    }
};

string getStateName(ProcessState state)
{
    switch (state)
    {
        case ProcessState::New: return "New";
        case ProcessState::Ready: return "Ready";
        case ProcessState::Running: return "Running";
        case ProcessState::Waiting: return "Waiting";
        case ProcessState::Terminated: return "Terminated";
    }

    return "Unknown";
}

void displayProcess(const Process& p)
{
    cout << "PID: " << p.pid
         << " | Name: " << p.name
         << " | Priority: " << p.priority
         << " | State: " << getStateName(p.state)
         << endl;
}

int main()
{
    vector<Process> processes;

    processes.push_back(
        Process(101, "Chrome", 2, ProcessState::Ready)
    );

    processes.push_back(
        Process(102, "Firefox", 1, ProcessState::Running)
    );

    processes.push_back(
        Process(103, "VS Code", 3, ProcessState::Waiting)
    );

    cout << "PROCESS TABLE" << endl;
    cout << "-----------------------------" << endl;

    for (const Process& p : processes)
    {
        displayProcess(p);
    }

    cout << "\nTotal processes: " << processes.size() << endl;

    return 0;
}
