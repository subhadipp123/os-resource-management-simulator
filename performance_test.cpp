
#include <iostream>

#include "performance/SchedulingBenchmark.h"
#include "performance/SchedulingReport.h"

using namespace std;

int main()
{
    SchedulingBenchmarkConfig config;

    config.processCount = 100;
    config.repetitions = 10;
    config.randomSeed = 42;

    SchedulingBenchmarkReport report =
        runSchedulingBenchmark(config);

    if (report.algorithmResults.size() != 5)
    {
        cout << "Benchmark failed!\n";
        return 1;
    }

    displaySchedulingReport(report);

    cout << "\nBenchmark completed successfully.\n";

    return 0;
}
