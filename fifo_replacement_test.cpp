
#include <iostream>

#include "virtual_memory/VirtualMemoryManager.h"
#include "virtual_memory/FIFOReplacement.h"

using namespace std;

void displayFrames(const VirtualMemoryManager& manager)
{
    cout << "\nPhysical Frames:\n";

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
}

int main()
{
    cout << "FIFO PAGE REPLACEMENT TEST\n";
    cout << "==========================\n\n";

    VirtualMemoryManager manager(5, 3);
    FIFOReplacement fifo;

    // Fill all three physical frames.
    int initialPages[] = {0, 1, 2};

    for (int page : initialPages)
    {
        manager.accessPage(page);
    }

    // Access Page 0 again. This is a hit.
    manager.accessPage(0);

    displayFrames(manager);

    // Request Page 3. All frames are occupied.
    cout << "\nRequesting Page 3:\n";

    PageAccessResult result = manager.accessPage(3);

    if (result == PageAccessResult::NO_FREE_FRAME)
    {
        cout << "Page Fault: No free frame available.\n";

        int victimFrame =
            fifo.selectVictimFrame(manager.getFrames());

        cout << "FIFO selected Frame: "
             << victimFrame << '\n';

        int victimPage =
            manager.getFrames()[victimFrame].pageNumber;

        cout << "Victim Page: "
             << victimPage << '\n';

        if (manager.replacePage(3, victimFrame))
        {
            cout << "Page " << victimPage
                 << " replaced with Page 3 successfully.\n";
        }
        else
        {
            cout << "Page replacement failed.\n";
        }
    }

    displayFrames(manager);

    cout << "\nUPDATED PAGE TABLE:\n";

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

    cout << "\nPERFORMANCE STATISTICS:\n";

    cout << "Page Faults: "
         << manager.getPageFaults() << '\n';

    cout << "Page Hits: "
         << manager.getPageHits() << '\n';

    cout << "Page Fault Rate: "
         << manager.getPageFaultRate() << "%\n";

    cout << "Page Hit Ratio: "
         << manager.getPageHitRatio() << "%\n";

    return 0;
}
