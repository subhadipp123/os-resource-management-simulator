
#include <iostream>
#include <vector>
#include <string>

#include "virtual_memory/FIFOPageReplacementSimulator.h"
#include "virtual_memory/LRUPageReplacementSimulator.h"
#include "virtual_memory/OptimalPageReplacementSimulator.h"

using namespace std;

template <typename Simulator>
void testSimulator(
    const string& name,
    const Simulator& simulator
)
{
    vector<int> references = {
        10,
        500000,
        10,
        2000000,
        500000
    };

    PageReplacementReport report =
        simulator.run(references, 3);

    cout << "\n" << name << '\n';

    for (const PageReplacementStep& step : report.steps)
    {
        cout << "Page " << step.referencePage << " | Frames: ";

        for (int page : step.frameContents)
        {
            if (page == -1)
                cout << "- ";
            else
                cout << page << " ";
        }

        cout << "| "
             << (step.pageHit ? "HIT" : "FAULT")
             << '\n';
    }

    cout << "Hits: " << report.pageHits << '\n';
    cout << "Faults: " << report.pageFaults << '\n';

    bool correct =
        report.pageHits == 2 &&
        report.pageFaults == 3 &&
        report.steps[0].referencePage == 10 &&
        report.steps[1].referencePage == 500000 &&
        report.steps[2].referencePage == 10 &&
        report.steps[3].referencePage == 2000000 &&
        report.steps[4].referencePage == 500000;

    cout << (correct ? "[PASS]" : "[FAIL]")
         << " Large page-number handling\n";
}

int main()
{
    FIFOPageReplacementSimulator fifo;
    LRUPageReplacementSimulator lru;
    OptimalPageReplacementSimulator optimal;

    testSimulator("FIFO", fifo);
    testSimulator("LRU", lru);
    testSimulator("OPTIMAL", optimal);

    return 0;
}
