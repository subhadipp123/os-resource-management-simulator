#ifndef SCHEDULING_BENCHMARK_H
#define SCHEDULING_BENCHMARK_H

#include <string>
#include <vector>

#include "../scheduling/SchedulingProcess.h"

struct BenchmarkMetrics
{
    double averageWaitingTime = 0.0;
    double averageTurnaroundTime = 0.0;
    double averageResponseTime = 0.0;

    double cpuUtilizationPercent = 0.0;
    double throughput = 0.0;
};

struct AlgorithmBenchmarkResult
{
    std::string algorithmName;

    BenchmarkMetrics metrics;

    double medianExecutionTimeMicroseconds = 0.0;
    double minimumExecutionTimeMicroseconds = 0.0;
    double maximumExecutionTimeMicroseconds = 0.0;
};

struct SchedulingBenchmarkConfig
{
    int processCount = 100;
    int repetitions = 10;
    unsigned int randomSeed = 42;
};

struct SchedulingBenchmarkReport
{
    int processCount = 0;
    int repetitions = 0;

    // False for manual comparisons; true for automatic benchmarks.
    bool includesExecutionTime = false;

    std::vector<AlgorithmBenchmarkResult> algorithmResults;
};

// Runs all five algorithms on a generated workload and measures execution time.
SchedulingBenchmarkReport runSchedulingBenchmark(
    const SchedulingBenchmarkConfig& config
);

// Runs all five algorithms once on the user's manually entered workload.
SchedulingBenchmarkReport runManualComparison(
    const std::vector<SchedulingProcess>& processes
);

#endif