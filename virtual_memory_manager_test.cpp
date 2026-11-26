
#include <iostream>
#include <iomanip>

#include "virtual_memory/VirtualMemoryManager.h"

using namespace std;

void displayResult(int pageNumber, PageAccessResult result)
{
    cout << "Accessing Page " << pageNumber << ": ";

    switch (result)
    {
        case PageAccessResult::INVALID_PAGE:
            cout << "Invalid Page";
            break;

        case PageAccessResult::PAGE_HIT:
            cout << "Page Hit";
            break;

        case PageAccessResult::PAGE_FAULT_HANDLED:
            cout << "Page Fault - Loaded into RAM";
            break;

        case PageAccessResult::NO_FREE_FRAME:
            cout << "Page Fault - No Free Frame";
            break;
    }

    cout << '\n';
}

int main()
{
    cout << "VIRTUAL MEMORY MANAGER TEST\n";
    cout << "===========================\n\n";

    VirtualMemoryManager manager(5, 3);

    // Access virtual pages
    displayResult(0, manager.accessPage(0));
    displayResult(1, manager.accessPage(1));
    displayResult(0, manager.accessPage(0));
    displayResult(2, manager.accessPage(2));
    displayResult(3, manager.accessPage(3));

    cout << "\nPHYSICAL FRAME STATUS\n";

    for (const Frame& frame : manager.getFrames())
    {
        cout << "Frame " << frame.frameNumber << ": ";

        if (frame.occupied)
        {
            cout << "Page " << frame.pageNumber;
        }
        else
        {
            cout << "Free";
        }

        cout << '\n';
    }

    cout << "\nPAGE TABLE STATUS\n";

    for (int i = 0; i < manager.getTotalPages(); i++)
    {
        const PageTableEntry* entry =
            manager.getPageTable().getEntry(i);

        cout << "Page " << i << ": ";

        if (entry->present)
        {
            cout << "Frame " << entry->frameNumber;
        }
        else
        {
            cout << "Not Present";
        }

        cout << '\n';
    }

    cout << "\nPERFORMANCE STATISTICS\n";

    cout << "Page Faults: "
         << manager.getPageFaults() << '\n';

    cout << "Page Hits: "
         << manager.getPageHits() << '\n';

    cout << fixed << setprecision(2);

    cout << "Page Fault Rate: "
         << manager.getPageFaultRate() << "%\n";

    cout << "Page Hit Ratio: "
         << manager.getPageHitRatio() << "%\n";

    cout << "\nUNLOADING PAGE 1\n";

    cout << (manager.unloadPage(1) ? "Success" : "Failed")
         << '\n';

    cout << "Page 1 Present: "
         << (manager.getPageTable().isPagePresent(1)
             ? "Yes" : "No") << '\n';

    cout << "\nACCESSING PAGE 3 AGAIN\n";

    displayResult(3, manager.accessPage(3));

    return 0;
}
