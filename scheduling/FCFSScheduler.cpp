#include <algorithm>
#include "FCFSScheduler.h"

using namespace std;

vector<FCFSResult> FCFSScheduler::schedule(
    const vector<SchedulingProcess>& processes)
{
    vector<SchedulingProcess> sortedProcesses = processes;

    // Sort processes by arrival time.
    // stable_sort preserves input order when arrival times are equal.
    stable_sort(
        sortedProcesses.begin(),
        sortedProcesses.end(),
        [](const SchedulingProcess& a, const SchedulingProcess& b)
        {
            return a.arrivalTime < b.arrivalTime;
        }
    );

    vector<FCFSResult> results;

    int currentTime = 0;

    for (const SchedulingProcess& process : sortedProcesses)
    {
        FCFSResult result;

        result.pid = process.pid;
        result.arrivalTime = process.arrivalTime;
        result.burstTime = process.burstTime;

        // CPU waits if the next process has not arrived yet.
        result.startTime = max(currentTime, process.arrivalTime);

        result.completionTime =
            result.startTime + process.burstTime;

        result.turnaroundTime =
            result.completionTime - process.arrivalTime;

        result.waitingTime =
            result.turnaroundTime - process.burstTime;

        result.responseTime =
            result.startTime - process.arrivalTime;

        currentTime = result.completionTime;

        results.push_back(result);
    }

    return results;
}