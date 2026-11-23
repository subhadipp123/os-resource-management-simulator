
#include <iostream>
#include <vector>

#include "performance/ManualInput.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes;

    if (!getManualProcesses(processes))
    {
        cout << "\nProcess input cancelled.\n";
        return 1;
    }

    cout << "\n========== ENTERED PROCESSES ==========\n";

    cout << "PID\tAT\tBT\tPriority\n";

    for (const SchedulingProcess& process : processes)
    {
        cout << process.pid << "\t"
             << process.arrivalTime << "\t"
             << process.burstTime << "\t"
             << process.priority << "\n";
    }

    cout << "\nTotal processes: " << processes.size() << "\n";

    return 0;
}
