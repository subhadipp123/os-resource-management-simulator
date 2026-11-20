
#ifndef GANTT_CHART_H
#define GANTT_CHART_H

#include <vector>
#include "SchedulingOutcome.h"

using namespace std;

// Displays the CPU execution timeline as an ASCII Gantt Chart.
void displayGanttChart(
    const vector<ExecutionSegment>& timeline
);

#endif
