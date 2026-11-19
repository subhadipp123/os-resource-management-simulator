#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <vector>
#include "SchedulingProcess.h"

using namespace std;

class FCFSScheduler
{
public:
    vector<FCFSResult> schedule(
        const vector<SchedulingProcess>& processes
    );
};

#endif