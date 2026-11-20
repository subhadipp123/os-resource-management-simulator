
#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <vector>
#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"

using namespace std;

class FCFSScheduler
{
public:
    SchedulingOutcome schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif
