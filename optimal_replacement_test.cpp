
#include <iostream>
#include <vector>

#include "virtual_memory/VirtualMemoryManager.h"
#include "virtual_memory/OptimalReplacement.h"

using namespace std;

int main()
{
    vector<int> referenceString = {
        1, 2, 3, 4, 1, 2, 5
    };

    VirtualMemoryManager manager(6, 3);
    OptimalReplacement optimal;

    for (int i = 0; i < static_cast<int>(referenceString.size()); i++)
    {
        int page = referenceString[i];

        PageAccessResult result = manager.accessPage(page);

        if (result == PageAccessResult::NO_FREE_FRAME)
        {
            int victimFrame = optimal.selectVictimFrame(
                manager.getFrames(),
                referenceString,
                i
            );

            if (victimFrame == -1 ||
                !manager.replacePage(page, victimFrame))
            {
                cout << "Replacement failed.\n";
                return 1;
            }
        }

        cout << "Page " << page << ": ";

        for (const Frame& frame : manager.getFrames())
        {
            if (frame.occupied)
                cout << frame.pageNumber << " ";
            else
                cout << "- ";
        }

        cout << '\n';
    }

    cout << "\nPage Hits: " << manager.getPageHits();
    cout << "\nPage Faults: " << manager.getPageFaults() << '\n';

    return 0;
}
