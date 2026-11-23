
#include <iostream>
#include <vector>

#include "performance/ManualInput.h"
#include "performance/SchedulingBenchmark.h"
#include "performance/SchedulingReport.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes;

    if (!getManualProcesses(processes))
    {
        cout << "\nManual comparison cancelled.\n";
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

    SchedulingBenchmarkReport report =
        runManualComparison(processes);

    if (report.algorithmResults.size() != 5)
    {
        cout << "\nManual comparison failed.\n";
        return 1;
    }

    displaySchedulingReport(report);

    cout << "\nManual comparison completed successfully.\n";

    return 0;
}
