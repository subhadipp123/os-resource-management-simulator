
#include <iostream>
#include <vector>

#include "scheduling/PreemptivePriorityScheduler2.h"
#include "scheduling/GanttChart.h"

using namespace std;

int main()
{
    vector<SchedulingProcess> processes =
    {
        {1, 0, 8, 3},
        {2, 2, 4, 1},
        {3, 3, 2, 2},
        {4, 4, 1, 4}
    };

    PreemptivePriorityScheduler2 scheduler;

    SchedulingOutcome outcome = scheduler.schedule(processes);

    if (!outcome.isValid)
    {
        cout << "Scheduling error: "
             << outcome.errorMessage << '\n';

        return 1;
    }

    cout << "\nPreemptive Priority Scheduling";
    cout << "\nDisplaying Gantt Chart...\n";

    displayGanttChart(outcome.executionTimeline);

    return 0;
}
