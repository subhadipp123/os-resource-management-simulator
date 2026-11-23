
#include "ManualInput.h"

#include <iostream>
#include <limits>

using namespace std;

// Reads an integer and validates its minimum allowed value.
bool readInteger(
    const string& prompt,
    int& value,
    int minimumValue
)
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

            cout << "Invalid value! Enter a number greater than or equal to "
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

// Collects process details from the user.
bool getManualProcesses(
    vector<SchedulingProcess>& processes
)
{
    processes.clear();

    int processCount = 0;

    cout << "\n========== MANUAL PROCESS INPUT ==========\n";

    if (!readInteger(
            "Enter number of processes (1-100): ",
            processCount,
            1
        ))
    {
        return false;
    }

    while (processCount > 100)
    {
        cout << "Maximum allowed processes: 100.\n";

        if (!readInteger(
                "Enter number of processes (1-100): ",
                processCount,
                1
            ))
        {
            return false;
        }
    }

    processes.reserve(processCount);

    for (int i = 0; i < processCount; i++)
    {
        SchedulingProcess process{};

        process.pid = i + 1;

        cout << "\nProcess P" << process.pid << "\n";

        if (!readInteger("Arrival Time: ", process.arrivalTime, 0))
        {
            processes.clear();
            return false;
        }

        if (!readInteger("Burst Time: ", process.burstTime, 1))
        {
            processes.clear();
            return false;
        }

        if (!readInteger("Priority: ", process.priority, 0))
        {
            processes.clear();
            return false;
        }

        processes.push_back(process);
    }

    cout << "\nProcess input completed successfully.\n";

    return true;
}
