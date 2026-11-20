
#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include <vector>
#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"

using namespace std;

class SJFScheduler
{
public:
    SchedulingOutcome schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif
