#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "GanttChart.h"

using namespace std;

void displayGanttChart(const vector<ExecutionSegment>& timeline)
{
    cout << "\n========== GANTT CHART ==========\n";

    if (timeline.empty())
    {
        cout << "No execution timeline available.\n";
        cout << "=================================\n";
        return;
    }

    vector<string> labels;
    labels.reserve(timeline.size());

    size_t maxTimeWidth = 1;

    // Find the widest timestamp.
    for (const auto& segment : timeline)
    {
        maxTimeWidth = max(
            maxTimeWidth,
            to_string(segment.startTime).length()
        );

        maxTimeWidth = max(
            maxTimeWidth,
            to_string(segment.endTime).length()
        );

        // PID 0 represents CPU idle time.
        if (segment.pid == 0)
        {
            labels.push_back("IDLE");
        }
        else
        {
            labels.push_back("P" + to_string(segment.pid));
        }
    }

    // Determine a suitable width for every segment.
    size_t cellWidth = maxTimeWidth + 1;

    for (const string& label : labels)
    {
        cellWidth = max(
            cellWidth,
            label.length() + 2
        );
    }

    // Print the top border.
    cout << "+";

    for (size_t i = 0; i < timeline.size(); i++)
    {
        cout << string(cellWidth, '-') << "+";
    }

    cout << "\n";

    // Print process labels, centered inside each segment.
    cout << "|";

    for (const string& label : labels)
    {
        size_t totalPadding = cellWidth - label.length();
        size_t leftPadding = totalPadding / 2;
        size_t rightPadding = totalPadding - leftPadding;

        cout << string(leftPadding, ' ')
             << label
             << string(rightPadding, ' ')
             << "|";
    }

    cout << "\n";

    // Print the bottom border.
    cout << "+";

    for (size_t i = 0; i < timeline.size(); i++)
    {
        cout << string(cellWidth, '-') << "+";
    }

    cout << "\n";

    // Print timestamps directly below their corresponding boundaries.
    for (const auto& segment : timeline)
    {
        cout << left
             << setw(static_cast<int>(cellWidth + 1))
             << segment.startTime;
    }

    cout << timeline.back().endTime << "\n";

    cout << "=================================\n";
}