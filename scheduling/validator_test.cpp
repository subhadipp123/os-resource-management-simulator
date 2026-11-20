
#include <iostream>
#include <vector>

#include "SchedulingValidator.h"

using namespace std;

void runTest(
    const string& testName,
    const vector<SchedulingProcess>& processes)
{
    ValidationResult result = validateProcesses(processes);

    cout << "\n" << testName << '\n';

    if (result.isValid)
    {
        cout << "Valid input.\n";
    }
    else
    {
        cout << "Error: " << result.errorMessage << '\n';
    }
}

int main()
{
    // Test 1: Valid processes
    vector<SchedulingProcess> validProcesses =
    {
        {1, 0, 5},
        {2, 3, 4}
    };

    runTest("Test 1: Valid processes", validProcesses);

    // Test 2: Invalid PID
    vector<SchedulingProcess> invalidPID =
    {
        {0, 0, 5}
    };

    runTest("Test 2: Invalid PID", invalidPID);

    // Test 3: Duplicate PID
    vector<SchedulingProcess> duplicatePID =
    {
        {1, 0, 5},
        {1, 2, 3}
    };

    runTest("Test 3: Duplicate PID", duplicatePID);

    // Test 4: Negative arrival time
    vector<SchedulingProcess> negativeArrival =
    {
        {1, -2, 5}
    };

    runTest("Test 4: Negative arrival time", negativeArrival);

    // Test 5: Zero burst time
    vector<SchedulingProcess> zeroBurst =
    {
        {1, 0, 0}
    };

    runTest("Test 5: Zero burst time", zeroBurst);

    // Test 6: Empty input
    vector<SchedulingProcess> emptyProcesses = {};

    runTest("Test 6: Empty input", emptyProcesses);

    return 0;
}
