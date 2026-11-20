
#include <vector>

#include "SJFScheduler.h"
#include "SchedulingValidator.h"

using namespace std;

SchedulingOutcome SJFScheduler::schedule(
    const vector<SchedulingProcess>& processes)
{
    // Validate input before scheduling.
    ValidationResult validation = validateProcesses(processes);

    if (!validation.isValid)
    {
        return {false, validation.errorMessage, {}, {}};
    }

    vector<Result> results;
    vector<ExecutionSegment> executionTimeline;

    int n = static_cast<int>(processes.size());
    int currentTime = 0;

    vector<bool> completed(n, false);

    int completedCount = 0;

    while (completedCount < n)
    {
        int selectedIndex = -1;

        // Find the shortest available process.
        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                if (selectedIndex == -1 ||
                    processes[i].burstTime <
                        processes[selectedIndex].burstTime ||
                    (processes[i].burstTime ==
                         processes[selectedIndex].burstTime &&
                     processes[i].arrivalTime <
                         processes[selectedIndex].arrivalTime))
                {
                    selectedIndex = i;
                }
            }
        }

        // If no process is ready, advance to the next arrival.
        if (selectedIndex == -1)
        {
            int nextArrival = -1;

            for (int i = 0; i < n; i++)
            {
                if (!completed[i])
                {
                    if (nextArrival == -1 ||
                        processes[i].arrivalTime < nextArrival)
                    {
                        nextArrival = processes[i].arrivalTime;
                    }
                }
            }

            // Record CPU idle interval.
            if (nextArrival > currentTime)
            {
                executionTimeline.push_back({
                    0,
                    currentTime,
                    nextArrival
                });
            }

            currentTime = nextArrival;
            continue;
        }

        const SchedulingProcess& process =
            processes[selectedIndex];

        Result result;

        result.pid = process.pid;
        result.arrivalTime = process.arrivalTime;
        result.burstTime = process.burstTime;

        result.startTime = currentTime;

        result.completionTime =
            result.startTime + result.burstTime;

        result.turnaroundTime =
            result.completionTime - result.arrivalTime;

        result.waitingTime =
            result.turnaroundTime - result.burstTime;

        result.responseTime =
            result.startTime - result.arrivalTime;

        // Record process execution interval.
        executionTimeline.push_back({
            process.pid,
            result.startTime,
            result.completionTime
        });

        currentTime = result.completionTime;

        results.push_back(result);

        completed[selectedIndex] = true;
        completedCount++;
    }

    return {
        true,
        "",
        results,
        executionTimeline
    };
}
