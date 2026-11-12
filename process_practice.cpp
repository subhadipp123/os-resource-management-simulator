
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

// Strict integer input function
bool readInteger(const string& prompt, int& value)
{
    string input;
    stringstream ss;

    cout << prompt;
    getline(cin, input);

    ss.str(input);

    if (!(ss >> value))
    {
        return false;
    }

    // Check for any extra non-whitespace characters
    ss >> ws;

    return ss.eof();
}

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

void createProcesses(vector<Process>& processes, int& nextPID)
{
    int count;

    while (true)
    {
        if (readInteger(
                "How many processes do you want to create? ", count)
            && count >= 1 && count <= 100)
        {
            break;
        }

        cout << "Invalid input! Enter a whole number between 1 and 100.\n";
    }

    for (int i = 0; i < count; i++)
    {
        string name;
        int priority;

        cout << "\nEnter name for Process " << i + 1 << ": ";
        getline(cin, name);

        while (name.empty())
        {
            cout << "Name cannot be empty. Enter again: ";
            getline(cin, name);
        }

        while (true)
        {
            if (readInteger("Enter priority (1-10): ", priority)
                && priority >= 1 && priority <= 10)
            {
                break;
            }

            cout << "Invalid priority! Enter a whole number between 1 and 10.\n";
        }

        processes.push_back(
            Process(nextPID++, name, priority, ProcessState::New)
        );

        cout << "Process created successfully!\n";
    }
}

void displayProcessTable(const vector<Process>& processes)
{
    cout << "\n========== PROCESS TABLE ==========\n";

    for (const Process& p : processes)
    {
        displayProcess(p);
    }

    cout << "===================================\n";
    cout << "Total processes: " << processes.size() << endl;
}

void searchProcessByPID(const vector<Process>& processes)
{
    int searchPID;

    if (!readInteger("\nEnter PID to search: ", searchPID))
    {
        cout << "Invalid PID! Enter a whole number.\n";
        return;
    }

    for (const Process& p : processes)
    {
        if (p.pid == searchPID)
        {
            cout << "\nProcess found!\n";
            displayProcess(p);
            return;
        }
    }

    cout << "Process with PID " << searchPID << " not found.\n";
}

int main()
{
    vector<Process> processes;
    int nextPID = 1;

    cout << "OS RESOURCE MANAGEMENT SIMULATOR\n";
    cout << "================================\n";

    createProcesses(processes, nextPID);

    displayProcessTable(processes);

    searchProcessByPID(processes);

    return 0;
}
