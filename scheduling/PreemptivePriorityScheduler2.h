
#ifndef PREEMPTIVE_PRIORITY_SCHEDULER2_H
#define PREEMPTIVE_PRIORITY_SCHEDULER2_H

#include <vector>
#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"

using namespace std;

class PreemptivePriorityScheduler2
{
public:
    SchedulingOutcome schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif
