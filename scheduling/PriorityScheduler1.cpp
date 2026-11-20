
#include <limits>
#include <vector>

#include "PriorityScheduler1.h"
#include "SchedulingValidator.h"

using namespace std;

SchedulingOutcome PriorityScheduler1::schedule(
    const vector<SchedulingProcess>& processes)
{
    // Validate input.
    ValidationResult validation = validateProcesses(processes);

    if (!validation.isValid)
    {
        return {false, validation.errorMessage, {}, {}};
    }

    int n = static_cast<int>(processes.size());

    vector<bool> completed(n, false);
    vector<Result> results;
    vector<ExecutionSegment> executionTimeline;

    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < n)
    {
        int selectedIndex = -1;

        // Find the highest-priority available process.
        // Lower priority number means higher priority.
        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                if (selectedIndex == -1 ||
                    processes[i].priority <
                        processes[selectedIndex].priority ||
                    (processes[i].priority ==
                         processes[selectedIndex].priority &&
                     processes[i].arrivalTime <
                         processes[selectedIndex].arrivalTime))
                {
                    selectedIndex = i;
                }
            }
        }

        // If no process is available, advance to the next arrival.
        if (selectedIndex == -1)
        {
            int nextArrival = numeric_limits<int>::max();

            for (int i = 0; i < n; i++)
            {
                if (!completed[i])
                {
                    if (processes[i].arrivalTime < nextArrival)
                    {
                        nextArrival = processes[i].arrivalTime;
                    }
                }
            }

            // Record CPU idle time.
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

        // Record the complete execution interval.
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
