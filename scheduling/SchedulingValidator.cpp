#include <unordered_set>

#include "SchedulingValidator.h"

using namespace std;

ValidationResult validateProcesses(
    const vector<SchedulingProcess>& processes)
{
    unordered_set<int> usedPIDs;

    for (const SchedulingProcess& process : processes)
    {
        // Check whether PID is positive.
        if (process.pid <= 0)
        {
            return {false, "PID must be positive."};
        }

        // Check whether PID is unique.
        if (usedPIDs.find(process.pid) != usedPIDs.end())
        {
            return {false, "Duplicate process ID found."};
        }

        usedPIDs.insert(process.pid);

        // Check whether arrival time is valid.
        if (process.arrivalTime < 0)
        {
            return {false, "Arrival time cannot be negative."};
        }

        // Check whether burst time is valid.
        if (process.burstTime <= 0)
        {
            return {false, "Burst time must be greater than zero."};
        }
    }

    return {true, ""};
}
