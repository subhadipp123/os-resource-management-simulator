#ifndef SCHEDULING_VALIDATOR_H
#define SCHEDULING_VALIDATOR_H

#include <string>
#include <vector>
#include "SchedulingProcess.h"

using namespace std;

struct ValidationResult
{
    bool isValid;
    string errorMessage;
};

ValidationResult validateProcesses(
    const vector<SchedulingProcess>& processes
);

#endif
