#include <iostream>
#include "process/ProcessManager.h"

using namespace std;

int main()
{
    ProcessManager manager;

    cout << "===== PROCESS MANAGER TESTS =====\n";

    // Test process creation
    cout << "\n1. Creating processes:\n";

    cout << "Create Chrome: "
         << manager.createProcess("Chrome", 5) << endl;

    cout << "Create VS Code: "
         << manager.createProcess("VS Code", 7) << endl;

    // Test invalid process creation
    cout << "\n2. Testing invalid creation:\n";

    cout << "Empty name: "
         << manager.createProcess("", 5) << endl;

    cout << "Invalid priority: "
         << manager.createProcess("Firefox", 15) << endl;

    // Display processes
    cout << "\n3. Displaying processes:\n";

    for (const Process& process : manager.getAllProcesses())
    {
        cout << "PID: " << process.pid
             << " | Name: " << process.name
             << " | Priority: " << process.priority
             << " | State: " << getStateName(process.state)
             << endl;
    }

    // Test searching
    cout << "\n4. Testing process search:\n";

    const Process* found = manager.findProcessByPID(1);

    cout << "Search PID 1: "
         << (found != nullptr) << endl;

    cout << "Search PID 99: "
         << (manager.findProcessByPID(99) != nullptr) << endl;

    // Test state transitions
    cout << "\n5. Testing state transitions:\n";

    cout << "New -> Ready: "
         << manager.changeProcessState(1, ProcessState::Ready)
         << endl;

    cout << "Ready -> Running: "
         << manager.changeProcessState(1, ProcessState::Running)
         << endl;

    cout << "Running -> Waiting: "
         << manager.changeProcessState(1, ProcessState::Waiting)
         << endl;

    cout << "Waiting -> Running: "
         << manager.changeProcessState(1, ProcessState::Running)
         << endl;

    // Test termination
    cout << "\n6. Testing termination:\n";

    cout << "Terminate PID 2: "
         << manager.terminateProcess(2) << endl;

    cout << "Terminate PID 2 again: "
         << manager.terminateProcess(2) << endl;

    cout << "Change terminated PID 2: "
         << manager.changeProcessState(2, ProcessState::Ready) << endl;

    cout << "\n===== FINAL PROCESS TABLE =====\n";

    for (const Process& process : manager.getAllProcesses())
    {
        cout << "PID: " << process.pid
             << " | Name: " << process.name
             << " | Priority: " << process.priority
             << " | State: " << getStateName(process.state)
             << endl;
    }

    return 0;
}
