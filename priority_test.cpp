
#include <iomanip>
#include <iostream>
#include <vector>

#include "scheduling/PriorityScheduler1.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes =
    {
        {1, 0, 8, 3},
        {2, 2, 3, 1},
        {3, 3, 2, 2},
        {4, 4, 1, 4}
    };

    PriorityScheduler1 scheduler;

    SchedulingOutcome outcome = scheduler.schedule(processes);

    if (!outcome.isValid)
    {
        cout << "Scheduling error: "
             << outcome.errorMessage << '\n';

        return 1;
    }

    const vector<Result>& results = outcome.results;

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    double totalResponseTime = 0;

    cout << "\n===== NON-PREEMPTIVE PRIORITY SCHEDULING =====\n";

    cout << left
         << setw(8) << "PID"
         << setw(10) << "AT"
         << setw(10) << "BT"
         << setw(10) << "PR"
         << setw(10) << "ST"
         << setw(10) << "CT"
         << setw(10) << "TAT"
         << setw(10) << "WT"
         << setw(10) << "RT"
         << '\n';

    for (const Result& result : results)
    {
        // Find the original process by PID.
        int priority = 0;

        for (const SchedulingProcess& process : processes)
        {
            if (process.pid == result.pid)
            {
                priority = process.priority;
                break;
            }
        }

        cout << left
             << setw(8) << result.pid
             << setw(10) << result.arrivalTime
             << setw(10) << result.burstTime
             << setw(10) << priority
             << setw(10) << result.startTime
             << setw(10) << result.completionTime
             << setw(10) << result.turnaroundTime
             << setw(10) << result.waitingTime
             << setw(10) << result.responseTime
             << '\n';

        totalWaitingTime += result.waitingTime;
        totalTurnaroundTime += result.turnaroundTime;
        totalResponseTime += result.responseTime;
    }

    int count = static_cast<int>(results.size());

    if (count > 0)
    {
        cout << fixed << setprecision(2);

        cout << "\nAverage Waiting Time: "
             << totalWaitingTime / count << '\n';

        cout << "Average Turnaround Time: "
             << totalTurnaroundTime / count << '\n';

        cout << "Average Response Time: "
             << totalResponseTime / count << '\n';
    }

    // Display execution timeline.
    cout << "\nExecution Timeline:\n";

    for (const ExecutionSegment& segment :
         outcome.executionTimeline)
    {
        if (segment.pid == 0)
            cout << "IDLE: ";
        else
            cout << "P" << segment.pid << ": ";

        cout << segment.startTime
             << " - "
             << segment.endTime
             << '\n';
    }

    cout << "\n===============================================\n";

    return 0;
}
