
#include <iostream>
#include <vector>

#include "FCFSScheduler.h"
#include "SJFScheduler.h"

using namespace std;

template <typename Scheduler>
void checkInvalidInput(
    const string& schedulerName,
    Scheduler& scheduler)
{
    vector<SchedulingProcess> processes =
    {
        {1, 0, 5},
        {2, 3, 0}
    };

    SchedulingOutcome outcome = scheduler.schedule(processes);

    cout << schedulerName << ": ";

    if (!outcome.isValid)
    {
        cout << "Correctly rejected invalid input: "
             << outcome.errorMessage << '\n';
    }
    else
    {
        cout << "ERROR: invalid input was accepted.\n";
    }
}

int main()
{
    FCFSScheduler fcfs;
    SJFScheduler sjf;

    checkInvalidInput("FCFS", fcfs);
    checkInvalidInput("SJF", sjf);

    vector<SchedulingProcess> emptyProcesses;

    SchedulingOutcome emptyOutcome = sjf.schedule(emptyProcesses);

    cout << "SJF empty input: ";

    if (emptyOutcome.isValid && emptyOutcome.results.empty())
    {
        cout << "Correctly accepted empty input with no results.\n";
    }
    else
    {
        cout << "ERROR: unexpected empty-input behavior.\n";
    }

    return 0;
}
