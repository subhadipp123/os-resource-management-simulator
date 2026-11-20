
#ifndef SCHEDULING_OUTCOME_H
#define SCHEDULING_OUTCOME_H

#include <string>
#include <vector>
#include "SchedulingProcess.h"

using namespace std;

// Represents one continuous period of CPU execution.
struct ExecutionSegment
{
    int pid;        // 0 represents CPU idle time
    int startTime;
    int endTime;
};

struct SchedulingOutcome
{
    bool isValid;
    string errorMessage;

    // Final metrics for each process.
    vector<Result> results;

    // Complete CPU execution history, including idle periods.
    vector<ExecutionSegment> executionTimeline;
};

#endif
