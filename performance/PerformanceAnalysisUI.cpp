
#include "PerformanceAnalysisUI.h"

#include <iostream>
#include <limits>
#include <vector>

#include "ManualInput.h"
#include "SchedulingBenchmark.h"
#include "SchedulingReport.h"

using namespace std;

// Reads an integer within an allowed range.
bool readInteger(
    const string& prompt,
    int& value,
    int minimum,
    int maximum
)
{
    while (true)
    {
        cout << prompt;

        if (cin >> value)
        {
            if (value >= minimum && value <= maximum)
            {
                return true;
            }

            cout << "Enter a value between "
                 << minimum << " and " << maximum << ".\n";
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

// Runs manual comparison mode.
void runManualComparisonMode()
{
    vector<SchedulingProcess> processes;

    if (!getManualProcesses(processes))
    {
        cout << "\nManual comparison cancelled.\n";
        return;
    }

    SchedulingBenchmarkReport report =
        runManualComparison(processes);

    if (report.algorithmResults.size() != 5)
    {
        cout << "\nUnable to complete manual comparison.\n";
        return;
    }

    displaySchedulingReport(report);
}

// Runs automatic benchmark mode.
void runAutomaticBenchmarkMode()
{
    SchedulingBenchmarkConfig config;

    cout << "\n========== AUTOMATIC BENCHMARK ==========\n";

    if (!readInteger(
            "Number of processes (1-1000): ",
            config.processCount,
            1,
            1000
        ))
    {
        return;
    }

    if (!readInteger(
            "Number of repetitions (1-30): ",
            config.repetitions,
            1,
            30
        ))
    {
        return;
    }

    int seed = 42;

    if (!readInteger(
            "Random seed (0-2147483647): ",
            seed,
            0,
            2147483647
        ))
    {
        return;
    }

    config.randomSeed = static_cast<unsigned int>(seed);

    cout << "\nRunning benchmark. Please wait...\n";

    SchedulingBenchmarkReport report =
        runSchedulingBenchmark(config);

    if (report.algorithmResults.size() != 5)
    {
        cout << "\nAutomatic benchmark failed.\n";
        return;
    }

    displaySchedulingReport(report);

    cout << "\nAutomatic benchmark completed successfully.\n";
}

// Displays the Performance Analysis submenu.
void performanceAnalysisMenu()
{
    int choice = 0;

    do
    {
        cout << "\n========================================\n";
        cout << "          PERFORMANCE ANALYSIS\n";
        cout << "========================================\n";

        cout << "\n1. Manual Input Comparison\n";
        cout << "2. Automatic Benchmark\n";
        cout << "3. Return to Main Menu\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                return;
            }

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                runManualComparisonMode();
                break;

            case 2:
                runAutomaticBenchmarkMode();
                break;

            case 3:
                cout << "\nReturning to Main Menu...\n";
                break;

            default:
                cout << "\nInvalid choice! Please select 1 to 3.\n";
        }

    } while (choice != 3);
}
