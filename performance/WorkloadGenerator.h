
#ifndef WORKLOAD_GENERATOR_H
#define WORKLOAD_GENERATOR_H

#include <vector>

#include "../scheduling/SchedulingProcess.h"

// Generates a repeatable set of scheduling processes.
std::vector<SchedulingProcess> generateWorkload(
    int processCount,
    unsigned int randomSeed
);

#endif
