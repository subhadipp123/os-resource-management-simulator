
#include <algorithm>
#include <limits>
#include <vector>

#include "PreemptivePriorityScheduler2.h"
#include "SchedulingValidator.h"

using namespace std;

// Record an execution segment.
// Merge adjacent segments of the same process.
static void addExecutionSegment(
    vector<ExecutionSegment>& timeline,
    int pid,
    int startTime,
    int endTime)
{
    if (!timeline.empty() &&
        timeline.back().pid == pid &&
        timeline.back().endTime == startTime)
    {
        timeline.back().endTime = endTime;
    }
    else
    {
        timeline.push_back({
            pid,
            startTime,
            endTime
        });
    }
}

SchedulingOutcome PreemptivePriorityScheduler2::schedule(
    const vector<SchedulingProcess>& processes)
{
    // Validate input first.
    ValidationResult validation = validateProcesses(processes);

    if (!validation.isValid)
    {
        return {false, validation.errorMessage, {}, {}};
    }

    int n = static_cast<int>(processes.size());

    vector<int> remainingTime(n);
    vector<int> firstStartTime(n, -1);
    vector<bool> completed(n, false);

    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = processes[i].burstTime;
    }

    vector<Result> results;
    vector<ExecutionSegment> executionTimeline;

    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < n)
    {
        int selectedIndex = -1;

        // Select the available process with the highest priority.
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

        // If no process is ready, advance to the next arrival.
        if (selectedIndex == -1)
        {
            int nextArrival = numeric_limits<int>::max();

            for (int i = 0; i < n; i++)
            {
                if (!completed[i])
                {
                    nextArrival = min(
                        nextArrival,
                        processes[i].arrivalTime
                    );
                }
            }

            // Record CPU idle interval.
            addExecutionSegment(
                executionTimeline,
                0,
                currentTime,
                nextArrival
            );

            currentTime = nextArrival;
            continue;
        }

        // Record the first CPU start time.
        if (firstStartTime[selectedIndex] == -1)
        {
            firstStartTime[selectedIndex] = currentTime;
        }

        // Find the next arrival with strictly higher priority.
        int nextHigherPriorityArrival =
            numeric_limits<int>::max();

        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrivalTime > currentTime &&
                processes[i].priority <
                    processes[selectedIndex].priority)
            {
                nextHigherPriorityArrival = min(
                    nextHigherPriorityArrival,
                    processes[i].arrivalTime
                );
            }
        }

        // Run until completion or a higher-priority arrival.
        int executionTime = remainingTime[selectedIndex];

        if (nextHigherPriorityArrival !=
            numeric_limits<int>::max())
        {
            executionTime = min(
                executionTime,
                nextHigherPriorityArrival - currentTime
            );
        }

        int executionStart = currentTime;

        remainingTime[selectedIndex] -= executionTime;
        currentTime += executionTime;

        // Record this execution interval.
        addExecutionSegment(
            executionTimeline,
            processes[selectedIndex].pid,
            executionStart,
            currentTime
        );

        // Check whether the process has completed.
        if (remainingTime[selectedIndex] == 0)
        {
            completed[selectedIndex] = true;
            completedCount++;

            const SchedulingProcess& process =
                processes[selectedIndex];

            Result result;

            result.pid = process.pid;
            result.arrivalTime = process.arrivalTime;
            result.burstTime = process.burstTime;

            result.startTime = firstStartTime[selectedIndex];
            result.completionTime = currentTime;

            result.turnaroundTime =
                result.completionTime - result.arrivalTime;

            result.waitingTime =
                result.turnaroundTime - result.burstTime;

            result.responseTime =
                result.startTime - result.arrivalTime;

            results.push_back(result);
        }
    }

    return {
        true,
        "",
        results,
        executionTimeline
    };
}
