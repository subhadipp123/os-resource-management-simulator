
#include <iostream>
#include <vector>

#include "performance/WorkloadGenerator.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes =
        generateWorkload(5, 42);

    cout << "========== GENERATED WORKLOAD ==========\n";

    cout << "PID\tArrival\tBurst\tPriority\n";

    for (const auto& process : processes)
    {
        cout << process.pid << "\t"
             << process.arrivalTime << "\t"
             << process.burstTime << "\t"
             << process.priority << "\n";
    }

    cout << "\nTotal processes: " << processes.size() << "\n";

    return 0;
}
