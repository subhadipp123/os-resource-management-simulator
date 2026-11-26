
#include <iostream>
#include <vector>

#include "virtual_memory/FIFOPageReplacementSimulator.h"

using namespace std;

int main()
{
    FIFOPageReplacementSimulator fifo;

    vector<int> referenceString = {
        1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
    };

    PageReplacementReport threeFrames =
        fifo.run(referenceString, 3);

    PageReplacementReport fourFrames =
        fifo.run(referenceString, 4);

    cout << "BELADY'S ANOMALY - FIFO\n\n";

    cout << "Reference String: ";

    for (int page : referenceString)
        cout << page << " ";

    cout << "\n\n";

    cout << "Frames: 3 | Page Faults: "
         << threeFrames.pageFaults << '\n';

    cout << "Frames: 4 | Page Faults: "
         << fourFrames.pageFaults << '\n';

    if (fourFrames.pageFaults > threeFrames.pageFaults)
        cout << "\nBelady's Anomaly observed!\n";
    else
        cout << "\nBelady's Anomaly not observed.\n";

    return 0;
}
