#include <iostream>
#include <vector>
#include <iomanip>

#include "scheduling/FCFSScheduler.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes =
    {
        {1, 2, 4},
        {2, 5, 3},
        {3, 6, 2}
    };

    FCFSScheduler scheduler;

    vector<FCFSResult> results =
        scheduler.schedule(processes);

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    double totalResponseTime = 0;

    cout << "\n========== FCFS SCHEDULING ==========\n";

    cout << left
         << setw(8) << "PID"
         << setw(10) << "AT"
         << setw(10) << "BT"
         << setw(10) << "ST"
         << setw(10) << "CT"
         << setw(10) << "TAT"
         << setw(10) << "WT"
         << setw(10) << "RT"
         << '\n';

    for (const FCFSResult& result : results)
    {
        cout << left
             << setw(8) << result.pid
             << setw(10) << result.arrivalTime
             << setw(10) << result.burstTime
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

    cout << "\n=====================================\n";

    return 0;
}