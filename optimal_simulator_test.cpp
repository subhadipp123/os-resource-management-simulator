
#include <iostream>
#include <vector>

#include "virtual_memory/OptimalPageReplacementSimulator.h"

using namespace std;

int main()
{
    OptimalPageReplacementSimulator simulator;

    vector<int> referenceString = {
        1, 2, 3, 4, 1, 2, 5
    };

    int totalFrames = 3;

    PageReplacementReport report =
        simulator.run(referenceString, totalFrames);

    cout << "OPTIMAL PAGE REPLACEMENT TEST\n\n";

    cout << "Page\tF1\tF2\tF3\tStatus\n";

    for (const PageReplacementStep& step : report.steps)
    {
        cout << step.referencePage << "\t";

        for (int frame : step.frameContents)
        {
            if (frame == -1)
                cout << "-\t";
            else
                cout << frame << "\t";
        }

        cout << (step.pageHit ? "HIT" : "FAULT") << '\n';
    }

    cout << "\nPage Hits: " << report.pageHits;
    cout << "\nPage Faults: " << report.pageFaults;
    cout << "\nHit Ratio: " << report.hitRatio << "%";
    cout << "\nFault Rate: " << report.faultRate << "%\n";

    return 0;
}
