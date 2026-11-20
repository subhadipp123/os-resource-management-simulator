
#ifndef SCHEDULING_OUTCOME_H
#define SCHEDULING_OUTCOME_H

#include <string>
#include <vector>
#include "SchedulingProcess.h"

using namespace std;

struct SchedulingOutcome
{
    bool isValid;
    string errorMessage;
    vector<Result> results;
};

#endif
