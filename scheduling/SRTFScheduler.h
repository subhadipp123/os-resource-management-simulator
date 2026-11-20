
#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H

#include <vector>
#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"

using namespace std;

class SRTFScheduler
{
public:
    SchedulingOutcome schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif
