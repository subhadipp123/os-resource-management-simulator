#ifndef PRIORITY_SCHEDULER1_H
#define PRIORITY_SCHEDULER1_H

#include <vector>
#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"

using namespace std;

class PriorityScheduler1
{
public:
    SchedulingOutcome schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif
