
#include <iostream>
#include <stdexcept>

#include "virtual_memory/VirtualMemoryManager.h"

using namespace std;

int main()
{
    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const string& name)
    {
        if (condition)
        {
            cout << "[PASS] " << name << '\n';
            passed++;
        }
        else
        {
            cout << "[FAIL] " << name << '\n';
            failed++;
        }
    };

    // Test 1: Zero frames.
    try
    {
        VirtualMemoryManager manager(5, 0);
        check(false, "Zero frames rejected");
    }
    catch (const invalid_argument&)
    {
        check(true, "Zero frames rejected");
    }

    // Test 2: Negative frames.
    try
    {
        VirtualMemoryManager manager(5, -3);
        check(false, "Negative frames rejected");
    }
    catch (const invalid_argument&)
    {
        check(true, "Negative frames rejected");
    }

    // Test 3: Valid frames.
    try
    {
        VirtualMemoryManager manager(5, 3);

        check(
            manager.getTotalFrames() == 3,
            "Valid frame count accepted"
        );
    }
    catch (...)
    {
        check(false, "Valid frame count accepted");
    }

    cout << "\nPassed: " << passed;
    cout << "\nFailed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
