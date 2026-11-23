
#include "WorkloadGenerator.h"

#include <random>

std::vector<SchedulingProcess> generateWorkload(
    int processCount,
    unsigned int randomSeed
)
{
    std::vector<SchedulingProcess> processes;

    if (processCount <= 0)
    {
        return processes;
    }

    processes.reserve(processCount);

    std::mt19937 generator(randomSeed);

    std::uniform_int_distribution<int> arrivalDistribution(0, 100);
    std::uniform_int_distribution<int> burstDistribution(1, 20);
    std::uniform_int_distribution<int> priorityDistribution(0, 9);

    for (int i = 0; i < processCount; i++)
    {
        SchedulingProcess process;

        process.pid = i + 1;
        process.arrivalTime = arrivalDistribution(generator);
        process.burstTime = burstDistribution(generator);
        process.priority = priorityDistribution(generator);

        processes.push_back(process);
    }

    return processes;
}
