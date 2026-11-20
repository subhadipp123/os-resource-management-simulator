
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

#include "CPUSchedulingUI.h"

#include "SchedulingProcess.h"
#include "SchedulingOutcome.h"
#include "FCFSScheduler.h"
#include "SJFScheduler.h"
#include "SRTFScheduler.h"
#include "PriorityScheduler1.h"
#include "PreemptivePriorityScheduler2.h"
#include "GanttChart.h"

using namespace std;

// Read an integer safely with a minimum allowed value.
static bool readInteger(
    const string& prompt,
    int& value,
    int minimumValue)
{
    while (true)
    {
        cout << prompt;

        if (cin >> value)
        {
            if (value >= minimumValue)
            {
                return true;
            }

            cout << "Value must be at least "
                 << minimumValue << ".\n";
        }
        else
        {
            if (cin.eof())
            {
                return false;
            }

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid input! Please enter an integer.\n";
        }
    }
}

// Collect process information from the user.
static bool readProcesses(
    vector<SchedulingProcess>& processes,
    bool requiresPriority)
{
    int processCount;

    while (true)
    {
        if (!readInteger(
                "Enter number of processes (1-100): ",
                processCount,
                1))
        {
            return false;
        }

        if (processCount <= 100)
        {
            break;
        }

        cout << "Maximum 100 processes are allowed.\n";
    }

    processes.clear();

    for (int i = 0; i < processCount; i++)
    {
        SchedulingProcess process{};

        process.pid = i + 1;

        cout << "\nEnter details for P" << process.pid << ":\n";

        if (!readInteger(
                "Arrival Time: ",
                process.arrivalTime,
                0))
        {
            return false;
        }

        if (!readInteger(
                "Burst Time: ",
                process.burstTime,
                1))
        {
            return false;
        }

        if (requiresPriority)
        {
            if (!readInteger(
                    "Priority (lower number = higher priority): ",
                    process.priority,
                    0))
            {
                return false;
            }
        }
        else
        {
            process.priority = 0;
        }

        processes.push_back(process);
    }

    return true;
}

// Find the priority belonging to a PID.
static int getPriority(
    int pid,
    const vector<SchedulingProcess>& processes)
{
    for (const SchedulingProcess& process : processes)
    {
        if (process.pid == pid)
        {
            return process.priority;
        }
    }

    return 0;
}

// Display scheduling results and performance metrics.
static void displaySchedulingResults(
    const SchedulingOutcome& outcome,
    const vector<SchedulingProcess>& processes,
    bool showPriority)
{
    if (!outcome.isValid)
    {
        cout << "\nScheduling error: "
             << outcome.errorMessage << '\n';
        return;
    }

    const vector<Result>& results = outcome.results;

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    double totalResponseTime = 0;

    cout << "\n========== SCHEDULING RESULTS ==========\n";

    cout << left
         << setw(8) << "PID"
         << setw(10) << "AT"
         << setw(10) << "BT";

    if (showPriority)
    {
        cout << setw(10) << "PR";
    }

    cout << setw(10) << "ST"
         << setw(10) << "CT"
         << setw(10) << "TAT"
         << setw(10) << "WT"
         << setw(10) << "RT"
         << '\n';

    for (const Result& result : results)
    {
        cout << left
             << setw(8) << result.pid
             << setw(10) << result.arrivalTime
             << setw(10) << result.burstTime;

        if (showPriority)
        {
            cout << setw(10)
                 << getPriority(result.pid, processes);
        }

        cout << setw(10) << result.startTime
             << setw(10) << result.completionTime
             << setw(10) << result.turnaroundTime
             << setw(10) << result.waitingTime
             << setw(10) << result.responseTime
             << '\n';

        totalWaitingTime += result.waitingTime;
        totalTurnaroundTime += result.turnaroundTime;
        totalResponseTime += result.responseTime;
    }

    int count = static_cast<int>(results.size());

    if (count > 0)
    {
        cout << fixed << setprecision(2);

        cout << "\nAverage Waiting Time: "
             << totalWaitingTime / count << '\n';

        cout << "Average Turnaround Time: "
             << totalTurnaroundTime / count << '\n';

        cout << "Average Response Time: "
             << totalResponseTime / count << '\n';
    }

    displayGanttChart(outcome.executionTimeline);
}

// Display the CPU scheduling submenu.
void cpuSchedulingMenu()
{
    int choice = 0;

    while (true)
    {
        cout << "\n========================================\n";
        cout << "             CPU SCHEDULING\n";
        cout << "========================================\n";

        cout << "\n1. FCFS (First Come First Serve)\n";
        cout << "2. SJF (Non-Preemptive)\n";
        cout << "3. SRTF (Preemptive)\n";
        cout << "4. Priority (Non-Preemptive)\n";
        cout << "5. Priority (Preemptive)\n";
        cout << "6. Return to Main Menu\n";

        if (!readInteger("\nEnter your choice: ", choice, 1))
        {
            return;
        }

        if (choice == 6)
        {
            cout << "Returning to Main Menu...\n";
            return;
        }

        if (choice < 1 || choice > 5)
        {
            cout << "Invalid choice! Please select 1 to 6.\n";
            continue;
        }

        vector<SchedulingProcess> processes;

        bool requiresPriority = (choice == 4 || choice == 5);

        if (!readProcesses(processes, requiresPriority))
        {
            return;
        }

        switch (choice)
        {
            case 1:
            {
                FCFSScheduler scheduler;
                SchedulingOutcome outcome = scheduler.schedule(processes);
                displaySchedulingResults(outcome, processes, false);
                break;
            }

            case 2:
            {
                SJFScheduler scheduler;
                SchedulingOutcome outcome = scheduler.schedule(processes);
                displaySchedulingResults(outcome, processes, false);
                break;
            }

            case 3:
            {
                SRTFScheduler scheduler;
                SchedulingOutcome outcome = scheduler.schedule(processes);
                displaySchedulingResults(outcome, processes, false);
                break;
            }

            case 4:
            {
                PriorityScheduler1 scheduler;
                SchedulingOutcome outcome = scheduler.schedule(processes);
                displaySchedulingResults(outcome, processes, true);
                break;
            }

            case 5:
            {
                PreemptivePriorityScheduler2 scheduler;
                SchedulingOutcome outcome = scheduler.schedule(processes);
                displaySchedulingResults(outcome, processes, true);
                break;
            }
        }
    }
}
