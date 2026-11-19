#include <iostream>
#include <string>
#include <sstream>
#include "ProcessManagementUI.h"
#include "ProcessManager.h"

using namespace std;

// Read a complete integer input and reject invalid characters
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

// Display all processes
void displayAllProcesses(const ProcessManager& manager)
{
    const vector<Process>& processes = manager.getAllProcesses();

    cout << "\n========== PROCESS TABLE ==========\n";

    if (processes.empty())
    {
        cout << "No processes available.\n";
    }
    else
    {
        for (const Process& process : processes)
        {
            cout << "PID: " << process.pid
                 << " | Name: " << process.name
                 << " | Priority: " << process.priority
                 << " | State: " << getStateName(process.state)
                 << '\n';
        }
    }

    cout << "===================================\n";
    cout << "Total processes: " << processes.size() << '\n';
}

// Create processes
void createProcesses(ProcessManager& manager)
{
    int count = 0;

    while (true)
    {
        if (readInteger(
                "\nHow many processes do you want to create? ",
                count)
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

        if (manager.createProcess(name, priority))
        {
            cout << "Process created successfully!\n";
        }
        else
        {
            cout << "Process creation failed. Check the process details.\n";
        }
    }
}

// Search for a process
void searchProcess(ProcessManager& manager)
{
    int pid = 0;

    if (!readInteger("\nEnter PID to search: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    const Process* process = manager.findProcessByPID(pid);

    if (process == nullptr)
    {
        cout << "Process with PID " << pid << " not found.\n";
        return;
    }

    cout << "\nProcess found!\n";
    cout << "PID: " << process->pid << '\n';
    cout << "Name: " << process->name << '\n';
    cout << "Priority: " << process->priority << '\n';
    cout << "State: " << getStateName(process->state) << '\n';
}

// Change process state
void changeProcessState(ProcessManager& manager)
{
    int pid = 0;

    if (!readInteger("\nEnter PID of the process: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    const Process* process = manager.findProcessByPID(pid);

    if (process == nullptr)
    {
        cout << "Process not found.\n";
        return;
    }

    cout << "\nCurrent State: "
         << getStateName(process->state) << '\n';

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

    if (manager.changeProcessState(pid, nextState))
    {
        cout << "Process state changed successfully!\n";
    }
    else
    {
        cout << "Invalid state transition or process operation.\n";
    }
}

// Terminate a process
void terminateProcess(ProcessManager& manager)
{
    int pid = 0;

    if (!readInteger("\nEnter PID to terminate: ", pid))
    {
        cout << "Invalid PID!\n";
        return;
    }

    if (manager.terminateProcess(pid))
    {
        cout << "Process terminated successfully!\n";
    }
    else
    {
        cout << "Termination failed. Process not found or already terminated.\n";
    }
}

// Interactive process management menu
void processManagementMenu()
{
    ProcessManager manager;
    int choice = 0;

    do
    {
        cout << "\n========== PROCESS MANAGEMENT ==========\n";
        cout << "1. Create Processes\n";
        cout << "2. Display All Processes\n";
        cout << "3. Search Process by PID\n";
        cout << "4. Change Process State\n";
        cout << "5. Terminate Process\n";
        cout << "6. Return to Main Menu\n";
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
                createProcesses(manager);
                break;

            case 2:
                displayAllProcesses(manager);
                break;

            case 3:
                searchProcess(manager);
                break;

            case 4:
                changeProcessState(manager);
                break;

            case 5:
                terminateProcess(manager);
                break;

            case 6:
                cout << "\nReturning to Main Menu.\n";
                break;

            default:
                cout << "Invalid choice! Select 1 to 6.\n";
        }

    } while (choice != 6 && !cin.eof());
}