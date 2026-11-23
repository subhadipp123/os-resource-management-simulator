#include "SchedulingBenchmark.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <vector>

#include "../scheduling/FCFSScheduler.h"
#include "../scheduling/SJFScheduler.h"
#include "../scheduling/SRTFScheduler.h"
#include "../scheduling/PriorityScheduler1.h"
#include "../scheduling/PreemptivePriorityScheduler2.h"
#include "../scheduling/SchedulingOutcome.h"

#include "WorkloadGenerator.h"

using namespace std;
using namespace std::chrono;

// Calculates scheduling metrics from a scheduler outcome.
BenchmarkMetrics calculateMetrics(
    const SchedulingOutcome& outcome
)
{
    BenchmarkMetrics metrics;

    if (outcome.results.empty())
    {
        return metrics;
    }

    double totalWaitingTime = 0.0;
    double totalTurnaroundTime = 0.0;
    double totalResponseTime = 0.0;
    double totalBurstTime = 0.0;

    for (const Result& result : outcome.results)
    {
        totalWaitingTime += result.waitingTime;
        totalTurnaroundTime += result.turnaroundTime;
        totalResponseTime += result.responseTime;
        totalBurstTime += result.burstTime;
    }

    const double processCount =
        static_cast<double>(outcome.results.size());

    metrics.averageWaitingTime =
        totalWaitingTime / processCount;

    metrics.averageTurnaroundTime =
        totalTurnaroundTime / processCount;

    metrics.averageResponseTime =
        totalResponseTime / processCount;

    int makespan = 0;

    for (const ExecutionSegment& segment : outcome.executionTimeline)
    {
        makespan = max(makespan, segment.endTime);
    }

    if (makespan > 0)
    {
        metrics.cpuUtilizationPercent =
            (totalBurstTime / makespan) * 100.0;

        metrics.throughput =
            processCount / makespan;
    }

    return metrics;
}

// Returns the median of measured execution times.
double calculateMedian(vector<double> executionTimes)
{
    if (executionTimes.empty())
    {
        return 0.0;
    }

    sort(executionTimes.begin(), executionTimes.end());

    const size_t middle = executionTimes.size() / 2;

    if (executionTimes.size() % 2 == 0)
    {
        return (
            executionTimes[middle - 1] +
            executionTimes[middle]
        ) / 2.0;
    }

    return executionTimes[middle];
}

// Runs one scheduler repeatedly and records its performance.
AlgorithmBenchmarkResult benchmarkAlgorithm(
    const string& algorithmName,
    const vector<SchedulingProcess>& processes,
    int repetitions,
    const function<SchedulingOutcome(
        const vector<SchedulingProcess>&
    )>& schedule
)
{
    AlgorithmBenchmarkResult result;
    result.algorithmName = algorithmName;

    vector<double> executionTimes;
    executionTimes.reserve(repetitions);

    SchedulingOutcome finalOutcome{};

    for (int i = 0; i < repetitions; i++)
    {
        const auto startTime = steady_clock::now();

        SchedulingOutcome outcome = schedule(processes);

        const auto endTime = steady_clock::now();

        const double elapsedTime =
            duration<double, micro>(endTime - startTime).count();

        executionTimes.push_back(elapsedTime);
        finalOutcome = outcome;

        if (!outcome.isValid)
        {
            result.metrics = BenchmarkMetrics{};
            return result;
        }
    }

    result.metrics = calculateMetrics(finalOutcome);

    sort(executionTimes.begin(), executionTimes.end());

    result.minimumExecutionTimeMicroseconds =
        executionTimes.front();

    result.maximumExecutionTimeMicroseconds =
        executionTimes.back();

    result.medianExecutionTimeMicroseconds =
        calculateMedian(executionTimes);

    return result;
}

// Runs one scheduler once without measuring execution time.
AlgorithmBenchmarkResult runManualAlgorithm(
    const string& algorithmName,
    const vector<SchedulingProcess>& processes,
    const function<SchedulingOutcome(
        const vector<SchedulingProcess>&
    )>& schedule
)
{
    AlgorithmBenchmarkResult result;
    result.algorithmName = algorithmName;

    const SchedulingOutcome outcome = schedule(processes);

    if (outcome.isValid)
    {
        result.metrics = calculateMetrics(outcome);
    }

    return result;
}

// Runs all five algorithms using one generated workload.
SchedulingBenchmarkReport runSchedulingBenchmark(
    const SchedulingBenchmarkConfig& config
)
{
    SchedulingBenchmarkReport report;

    if (config.processCount <= 0 || config.repetitions <= 0)
    {
        return report;
    }

    report.processCount = config.processCount;
    report.repetitions = config.repetitions;
    report.includesExecutionTime = true;

    const vector<SchedulingProcess> processes =
        generateWorkload(
            config.processCount,
            config.randomSeed
        );

    FCFSScheduler fcfsScheduler;
    SJFScheduler sjfScheduler;
    SRTFScheduler srtfScheduler;
    PriorityScheduler1 priorityScheduler;
    PreemptivePriorityScheduler2 preemptivePriorityScheduler;

    report.algorithmResults.push_back(
        benchmarkAlgorithm(
            "FCFS",
            processes,
            config.repetitions,
            [&fcfsScheduler](const vector<SchedulingProcess>& input)
            {
                return fcfsScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        benchmarkAlgorithm(
            "SJF",
            processes,
            config.repetitions,
            [&sjfScheduler](const vector<SchedulingProcess>& input)
            {
                return sjfScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        benchmarkAlgorithm(
            "SRTF",
            processes,
            config.repetitions,
            [&srtfScheduler](const vector<SchedulingProcess>& input)
            {
                return srtfScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        benchmarkAlgorithm(
            "Priority (Non-preemptive)",
            processes,
            config.repetitions,
            [&priorityScheduler](const vector<SchedulingProcess>& input)
            {
                return priorityScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        benchmarkAlgorithm(
            "Priority (Preemptive)",
            processes,
            config.repetitions,
            [&preemptivePriorityScheduler](
                const vector<SchedulingProcess>& input
            )
            {
                return preemptivePriorityScheduler.schedule(input);
            }
        )
    );

    return report;
}

// Runs all five algorithms once using the same manually entered processes.
SchedulingBenchmarkReport runManualComparison(
    const vector<SchedulingProcess>& processes
)
{
    SchedulingBenchmarkReport report;

    if (processes.empty())
    {
        return report;
    }

    report.processCount = static_cast<int>(processes.size());
    report.repetitions = 1;
    report.includesExecutionTime = false;

    FCFSScheduler fcfsScheduler;
    SJFScheduler sjfScheduler;
    SRTFScheduler srtfScheduler;
    PriorityScheduler1 priorityScheduler;
    PreemptivePriorityScheduler2 preemptivePriorityScheduler;

    report.algorithmResults.push_back(
        runManualAlgorithm(
            "FCFS",
            processes,
            [&fcfsScheduler](const vector<SchedulingProcess>& input)
            {
                return fcfsScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        runManualAlgorithm(
            "SJF",
            processes,
            [&sjfScheduler](const vector<SchedulingProcess>& input)
            {
                return sjfScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        runManualAlgorithm(
            "SRTF",
            processes,
            [&srtfScheduler](const vector<SchedulingProcess>& input)
            {
                return srtfScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        runManualAlgorithm(
            "Priority (Non-preemptive)",
            processes,
            [&priorityScheduler](const vector<SchedulingProcess>& input)
            {
                return priorityScheduler.schedule(input);
            }
        )
    );

    report.algorithmResults.push_back(
        runManualAlgorithm(
            "Priority (Preemptive)",
            processes,
            [&preemptivePriorityScheduler](
                const vector<SchedulingProcess>& input
            )
            {
                return preemptivePriorityScheduler.schedule(input);
            }
        )
    );

    return report;
}