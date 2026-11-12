
#include "Process.h"

using namespace std;

// Default constructor
Process::Process()
    : pid(0),
      name("Unknown"),
      priority(0),
      state(ProcessState::New)
{
}

// Parameterized constructor
Process::Process(int id, string processName, int p, ProcessState s)
    : pid(id),
      name(processName),
      priority(p),
      state(s)
{
}

// Convert process state to readable text
string getStateName(ProcessState state)
{
    switch (state)
    {
        case ProcessState::New:
            return "New";

        case ProcessState::Ready:
            return "Ready";

        case ProcessState::Running:
            return "Running";

        case ProcessState::Waiting:
            return "Waiting";

        case ProcessState::Terminated:
            return "Terminated";
    }

    return "Unknown";
}

// Validate state transitions
bool isValidTransition(ProcessState current, ProcessState next)
{
    if (current == ProcessState::Terminated)
    {
        return false;
    }

    if (next == ProcessState::Terminated)
    {
        return true;
    }

    switch (current)
    {
        case ProcessState::New:
            return next == ProcessState::Ready;

        case ProcessState::Ready:
            return next == ProcessState::Running;

        case ProcessState::Running:
            return next == ProcessState::Ready ||
                   next == ProcessState::Waiting;

        case ProcessState::Waiting:
            return next == ProcessState::Ready;

        case ProcessState::Terminated:
            return false;
    }

    return false;
}
