
#include <iostream>
#include <vector>

#include "virtual_memory/FIFOPageReplacementSimulator.h"
#include "virtual_memory/LRUPageReplacementSimulator.h"
#include "virtual_memory/OptimalPageReplacementSimulator.h"

using namespace std;

template <typename Simulator>
void compareFrames(
    const string& name,
    const Simulator& simulator,
    const vector<int>& referenceString
)
{
    PageReplacementReport threeFrames =
        simulator.run(referenceString, 3);

    PageReplacementReport fourFrames =
        simulator.run(referenceString, 4);

    cout << name << '\n';

    cout << "3 Frames: "
         << threeFrames.pageFaults << " page faults\n";

    cout << "4 Frames: "
         << fourFrames.pageFaults << " page faults\n\n";
}

int main()
{
    vector<int> referenceString = {
        1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
    };

    FIFOPageReplacementSimulator fifo;
    LRUPageReplacementSimulator lru;
    OptimalPageReplacementSimulator optimal;

    cout << "BELADY'S ANOMALY COMPARISON\n\n";

    compareFrames("FIFO", fifo, referenceString);
    compareFrames("LRU", lru, referenceString);
    compareFrames("OPTIMAL", optimal, referenceString);

    return 0;
}
