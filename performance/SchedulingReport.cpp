#include "SchedulingReport.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

void displaySchedulingReport(
    const SchedulingBenchmarkReport& report
)
{
    if (report.algorithmResults.empty())
    {
        cout << "\nNo comparison results available.\n";
        return;
    }

    cout << "\n========== CPU SCHEDULING COMPARISON ==========\n";

    cout << "Processes: " << report.processCount;

    if (report.includesExecutionTime)
    {
        cout << " | Repetitions: " << report.repetitions;
    }

    cout << "\n\n";

    cout << fixed << setprecision(2);

    cout << left << setw(27) << "Algorithm"
         << right
         << setw(12) << "Avg WT"
         << setw(12) << "Avg TAT"
         << setw(12) << "Avg RT"
         << setw(12) << "CPU Util%"
         << setw(13) << "Throughput";

    if (report.includesExecutionTime)
    {
        cout << setw(16) << "Median(us)";
    }

    cout << "\n";

    const int tableWidth =
        report.includesExecutionTime ? 104 : 88;

    cout << string(tableWidth, '-') << "\n";

    double lowestWaitingTime =
        numeric_limits<double>::max();

    double fastestExecutionTime =
        numeric_limits<double>::max();

    string bestWaitingAlgorithm;
    string fastestAlgorithm;

    for (const AlgorithmBenchmarkResult& result :
         report.algorithmResults)
    {
        cout << left << setw(27) << result.algorithmName
             << right
             << setw(12) << result.metrics.averageWaitingTime
             << setw(12) << result.metrics.averageTurnaroundTime
             << setw(12) << result.metrics.averageResponseTime
             << setw(12) << result.metrics.cpuUtilizationPercent
             << setw(13) << result.metrics.throughput;

        if (report.includesExecutionTime)
        {
            cout << setw(16)
                 << result.medianExecutionTimeMicroseconds;
        }

        cout << "\n";

        if (result.metrics.averageWaitingTime < lowestWaitingTime)
        {
            lowestWaitingTime =
                result.metrics.averageWaitingTime;

            bestWaitingAlgorithm = result.algorithmName;
        }

        if (report.includesExecutionTime &&
            result.medianExecutionTimeMicroseconds <
                fastestExecutionTime)
        {
            fastestExecutionTime =
                result.medianExecutionTimeMicroseconds;

            fastestAlgorithm = result.algorithmName;
        }
    }

    cout << string(tableWidth, '-') << "\n";

    cout << "\n========== PERFORMANCE HIGHLIGHTS ==========\n";

    cout << "Lowest Average Waiting Time: "
         << bestWaitingAlgorithm
         << " (" << lowestWaitingTime << ")\n";

    if (report.includesExecutionTime)
    {
        cout << "Fastest Algorithm Execution: "
             << fastestAlgorithm
             << " (" << fastestExecutionTime
             << " microseconds)\n";
    }

    cout << "\n============================================\n";
}