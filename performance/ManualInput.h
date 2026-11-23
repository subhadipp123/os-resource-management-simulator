
#ifndef MANUAL_INPUT_H
#define MANUAL_INPUT_H

#include <vector>

#include "../scheduling/SchedulingProcess.h"

// Reads and validates process details from the user.
bool getManualProcesses(
    std::vector<SchedulingProcess>& processes
);

#endif
