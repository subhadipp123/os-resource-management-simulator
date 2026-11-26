
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "virtual_memory/FIFOPageReplacementSimulator.h"
#include "virtual_memory/LRUPageReplacementSimulator.h"
#include "virtual_memory/OptimalPageReplacementSimulator.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, const string& testName)
{
    if (condition)
    {
        cout << "[PASS] " << testName << '\n';
        passed++;
    }
    else
    {
        cout << "[FAIL] " << testName << '\n';
        failed++;
    }
}

template <typename Simulator>
void testAlgorithm(const string& name, const Simulator& simulator)
{
    cout << "\nTesting " << name << '\n';

    // Test 1: All references are identical.
    {
        vector<int> references = {1, 1, 1, 1};

        PageReplacementReport report =
            simulator.run(references, 3);

        check(
            report.pageFaults == 1 && report.pageHits == 3,
            name + ": Repeated page references"
        );
    }

    // Test 2: Only one frame.
    {
        vector<int> references = {1, 2, 1, 2};

        PageReplacementReport report =
            simulator.run(references, 1);

        check(
            report.pageFaults == 4 && report.pageHits == 0,
            name + ": Single frame handling"
        );
    }

    // Test 3: Only one reference.
    {
        vector<int> references = {5};

        PageReplacementReport report =
            simulator.run(references, 3);

        check(
            report.pageFaults == 1 &&
            report.pageHits == 0 &&
            report.steps.size() == 1,
            name + ": Single reference handling"
        );
    }

    // Test 4: Frame count greater than unique pages.
    {
        vector<int> references = {1, 2, 1, 2, 3};

        PageReplacementReport report =
            simulator.run(references, 5);

        check(
            report.pageFaults == 3 && report.pageHits == 2,
            name + ": More frames than unique pages"
        );
    }

    // Test 5: Metrics must add up to total references.
    {
        vector<int> references = {1, 2, 3, 4, 1, 2, 5};

        PageReplacementReport report =
            simulator.run(references, 3);

        check(
            report.pageFaults + report.pageHits ==
                static_cast<int>(references.size()),
            name + ": Hits and faults total correctly"
        );

        check(
            report.steps.size() == references.size(),
            name + ": One trace step per reference"
        );
    }

    // Test 6: Empty reference string must throw.
    {
        bool exceptionThrown = false;

        try
        {
            simulator.run({}, 3);
        }
        catch (const invalid_argument&)
        {
            exceptionThrown = true;
        }

        check(
            exceptionThrown,
            name + ": Empty reference string rejected"
        );
    }

    // Test 7: Zero frames must throw.
    {
        bool exceptionThrown = false;

        try
        {
            simulator.run({1, 2, 3}, 0);
        }
        catch (const invalid_argument&)
        {
            exceptionThrown = true;
        }

        check(
            exceptionThrown,
            name + ": Zero frames rejected"
        );
    }

    // Test 8: Negative page number must throw.
    {
        bool exceptionThrown = false;

        try
        {
            simulator.run({1, -2, 3}, 2);
        }
        catch (const invalid_argument&)
        {
            exceptionThrown = true;
        }

        check(
            exceptionThrown,
            name + ": Negative page number rejected"
        );
    }
}

int main()
{
    FIFOPageReplacementSimulator fifo;
    LRUPageReplacementSimulator lru;
    OptimalPageReplacementSimulator optimal;

    testAlgorithm("FIFO", fifo);
    testAlgorithm("LRU", lru);
    testAlgorithm("OPTIMAL", optimal);

    cout << "\n====================================\n";
    cout << "PAGE REPLACEMENT EDGE CASE SUMMARY\n";
    cout << "====================================\n";

    cout << "Passed: " << passed << '\n';
    cout << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
