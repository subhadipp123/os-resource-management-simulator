
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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
          priority(0), state(ProcessState::New)
    {
    }

    Process(int id, string processName, int p, ProcessState s)
        : pid(id), name(processName),
          priority(p), state(s)
    {
    }
};

// Strict integer input
bool readInteger(const string& prompt, int& value)
{
    string input;
    stringstream ss;

    cout << prompt;

    if (!getline(cin, input))
    {
        return false;
    }

    ss.str(input);

    if (!(ss >> value))
    {
        return false;
    }

    ss >> ws;
    return ss.eof();
}

// Convert process state to readable text
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

// Display one process
void displayProcess(const Process& p)
{
    cout << "PID: " << p.pid
         << " | Name: " << p.name
         << " | Priority: " << p.priority
         << " | State: " << getStateName(p.state)
         << endl;
}

// Find a process using PID
Process* findProcessByPID(vector<Process>& processes, int pid)
{
    for (Process& p : processes)
    {
        if (p.pid == pid)
        {
            return &p;
        }
    }

    return nullptr;
}

// Create processes
void createProcesses(vector<Process>& processes, int& nextPID)
{
    int count = 0;

    while (true)
    {
        if (readInteger(
                "\nHow many processes do you want to create? ", count)
            && count >= 1 && count <= 100)
        {
            break;
        }

        if (cin.eof())
        {
            return;
        }

        cout << "Invalid input! Enter a whole number between 1 and 100.\n";
    }

    for (int i = 0; i < count; i++)
    {
        string name;
        int priority = 0;

        cout << "\nEnter name for Process " << i + 1 << ": ";

        if (!getline(cin, name))
        {
            return;
        }

        while (name.empty())
        {
            cout << "Name cannot be empty. Enter again: ";

            if (!getline(cin, name))
            {
                return;
            }
        }

        while (true)
        {
            if (readInteger("Enter priority (1-10): ", priority)
                && priority >= 1 && priority <= 10)
            {
                break;
            }

            if (cin.eof())
            {
                return;
            }

            cout << "Invalid priority! Enter a whole number between 1 and 10.\n";
        }

        processes.push_back(
            Process(nextPID++, name, priority, ProcessState::New)
        );

        cout << "Process created successfully!\n";
    }
}

// Display all processes
void displayProcessTable(const vector<Process>& processes)
{
    cout << "\n========== PROCESS TABLE ==========\n";

    if (processes.empty())
    {
        cout << "No processes available.\n";
    }
    else
    {
        for (const Process& p : processes)
        {
            displayProcess(p);
        }
    }

    cout << "===================================\n";
    cout << "Total processes: " << processes.size() << endl;
}

// Search for a process
void searchProcessByPID(const vector<Process>& processes)
{
    int pid = 0;

    if (!readInteger("\nEnter PID to search: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    for (const Process& p : processes)
    {
        if (p.pid == pid)
        {
            cout << "\nProcess found!\n";
            displayProcess(p);
            return;
        }
    }

    cout << "Process with PID " << pid << " not found.\n";
}

// Change process state
void changeProcessState(vector<Process>& processes)
{
    if (processes.empty())
    {
        cout << "\nNo processes available.\n";
        return;
    }

    int pid = 0;

    if (!readInteger("\nEnter PID of the process: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    Process* process = findProcessByPID(processes, pid);

    if (process == nullptr)
    {
        cout << "Process not found.\n";
        return;
    }

    cout << "\nCurrent State: "
         << getStateName(process->state) << endl;

    if (process->state == ProcessState::Terminated)
    {
        cout << "Terminated processes cannot change state.\n";
        return;
    }

    cout << "\nSelect New State:\n";
    cout << "1. Ready\n";
    cout << "2. Running\n";
    cout << "3. Waiting\n";

    int choice = 0;

    if (!readInteger("Enter choice: ", choice))
    {
        cout << "Invalid input!\n";
        return;
    }

    ProcessState nextState;

    switch (choice)
    {
        case 1:
            nextState = ProcessState::Ready;
            break;

        case 2:
            nextState = ProcessState::Running;
            break;

        case 3:
            nextState = ProcessState::Waiting;
            break;

        default:
            cout << "Invalid choice! Select 1 to 3.\n";
            return;
    }

    if (isValidTransition(process->state, nextState))
    {
        process->state = nextState;

        cout << "\nProcess state changed successfully!\n";
        cout << "New State: " << getStateName(process->state) << endl;
    }
    else
    {
        cout << "\nInvalid state transition!\n";
        cout << "Current State: " << getStateName(process->state) << endl;
    }
}

// Terminate a process
void terminateProcess(vector<Process>& processes)
{
    if (processes.empty())
    {
        cout << "\nNo processes available.\n";
        return;
    }

    int pid = 0;

    if (!readInteger("\nEnter PID to terminate: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    Process* process = findProcessByPID(processes, pid);

    if (process == nullptr)
    {
        cout << "Process not found.\n";
        return;
    }

    if (process->state == ProcessState::Terminated)
    {
        cout << "This process is already terminated.\n";
        return;
    }

    process->state = ProcessState::Terminated;

    cout << "\nProcess terminated successfully!\n";
    cout << "PID: " << process->pid << endl;
}

// Process management menu
void processManagementMenu()
{
    vector<Process> processes;
    int nextPID = 1;
    int choice = 0;

    do
    {
        cout << "\n========== PROCESS MANAGEMENT ==========\n";
        cout << "1. Create Processes\n";
        cout << "2. Display All Processes\n";
        cout << "3. Search Process by PID\n";
        cout << "4. Change Process State\n";
        cout << "5. Terminate Process\n";
        cout << "6. Exit Process Management\n";
        cout << "========================================\n";

        if (!readInteger("Enter your choice: ", choice))
        {
            if (cin.eof())
            {
                break;
            }

            cout << "Invalid input! Enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                createProcesses(processes, nextPID);
                break;

            case 2:
                displayProcessTable(processes);
                break;

            case 3:
                searchProcessByPID(processes);
                break;

            case 4:
                changeProcessState(processes);
                break;

            case 5:
                terminateProcess(processes);
                break;

            case 6:
                cout << "\nExiting Process Management.\n";
                break;

            default:
                cout << "Invalid choice! Select 1 to 6.\n";
        }

    } while (choice != 6 && !cin.eof());
}

int main()
{
    cout << "OS RESOURCE MANAGEMENT SIMULATOR\n";

    processManagementMenu();

    return 0;
}
