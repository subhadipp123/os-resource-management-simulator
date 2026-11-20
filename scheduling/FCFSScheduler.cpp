
#include <algorithm>

#include "FCFSScheduler.h"
#include "SchedulingValidator.h"

using namespace std;

SchedulingOutcome FCFSScheduler::schedule(
    const vector<SchedulingProcess>& processes)
{
    // Validate input before scheduling.
    ValidationResult validation = validateProcesses(processes);

    if (!validation.isValid)
    {
        return {false, validation.errorMessage, {}, {}};
    }

    vector<SchedulingProcess> sortedProcesses = processes;

    // Sort by arrival time.
    // stable_sort preserves input order for equal arrival times.
    stable_sort(
        sortedProcesses.begin(),
        sortedProcesses.end(),
        [](const SchedulingProcess& a, const SchedulingProcess& b)
        {
            return a.arrivalTime < b.arrivalTime;
        }
    );

    vector<Result> results;
    vector<ExecutionSegment> executionTimeline;

    int currentTime = 0;

    for (const SchedulingProcess& process : sortedProcesses)
    {
        Result result;

        result.pid = process.pid;
        result.arrivalTime = process.arrivalTime;
        result.burstTime = process.burstTime;

        // Record CPU idle time if the next process has not arrived.
        if (currentTime < process.arrivalTime)
        {
            executionTimeline.push_back({
                0,
                currentTime,
                process.arrivalTime
            });
        }

        result.startTime = max(currentTime, process.arrivalTime);

        result.completionTime =
            result.startTime + process.burstTime;

        result.turnaroundTime =
            result.completionTime - process.arrivalTime;

        result.waitingTime =
            result.turnaroundTime - process.burstTime;

        result.responseTime =
            result.startTime - process.arrivalTime;

        // Record this process's CPU execution interval.
        executionTimeline.push_back({
            process.pid,
            result.startTime,
            result.completionTime
        });

        currentTime = result.completionTime;

        results.push_back(result);
    }

    return {
        true,
        "",
        results,
        executionTimeline
    };
}
