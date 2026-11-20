
#include <algorithm>
#include <limits>
#include <vector>

#include "SRTFScheduler.h"
#include "SchedulingValidator.h"

using namespace std;

// Add an execution segment.
// Merge adjacent segments belonging to the same process.
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

SchedulingOutcome SRTFScheduler::schedule(
    const vector<SchedulingProcess>& processes)
{
    // Validate input before scheduling.
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

        // Select the arrived process with the shortest remaining time.
        // Ties: earlier arrival time, then original input order.
        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                if (selectedIndex == -1 ||
                    remainingTime[i] < remainingTime[selectedIndex] ||
                    (remainingTime[i] == remainingTime[selectedIndex] &&
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

            // Record CPU idle time.
            addExecutionSegment(
                executionTimeline,
                0,
                currentTime,
                nextArrival
            );

            currentTime = nextArrival;
            continue;
        }

        // Record the first time this process gets the CPU.
        if (firstStartTime[selectedIndex] == -1)
        {
            firstStartTime[selectedIndex] = currentTime;
        }

        // Find the next process arrival.
        int nextArrival = numeric_limits<int>::max();

        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrivalTime > currentTime)
            {
                nextArrival = min(
                    nextArrival,
                    processes[i].arrivalTime
                );
            }
        }

        // Run until completion or the next arrival.
        int executionTime = remainingTime[selectedIndex];

        if (nextArrival != numeric_limits<int>::max())
        {
            executionTime = min(
                executionTime,
                nextArrival - currentTime
            );
        }

        int executionStart = currentTime;

        remainingTime[selectedIndex] -= executionTime;
        currentTime += executionTime;

        // Record this CPU execution interval.
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

            // Results are stored in completion order.
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
