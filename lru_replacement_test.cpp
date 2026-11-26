#include <iostream>

#include "virtual_memory/VirtualMemoryManager.h"
#include "virtual_memory/LRUReplacement.h"

using namespace std;

int main()
{
    VirtualMemoryManager manager(5, 3);

    LRUReplacement lru;

    // Load pages 1, 2 and 3.
    manager.accessPage(1);
    manager.accessPage(2);
    manager.accessPage(3);

    // Access page 1 again.
    // This makes page 1 the most recently used.
    manager.accessPage(1);

    cout << "LRU PAGE REPLACEMENT TEST\n";
    cout << "=========================\n\n";

    cout << "Before replacement:\n";

    for (const Frame& frame : manager.getFrames())
    {
        cout << "Frame " << frame.frameNumber + 1
             << " -> Page " << frame.pageNumber
             << " | Last Used: " << frame.lastUsedTime
             << '\n';
    }

    // Select the least recently used frame.
    int victimFrame =
        lru.selectVictimFrame(manager.getFrames());

    if (victimFrame == -1)
    {
        cout << "\nNo occupied frame found.\n";
        return 1;
    }

    int replacedPage =
        manager.getFrames()[victimFrame].pageNumber;

    cout << "\nSelected victim: Page "
         << replacedPage << '\n';

    cout << "Selected frame: "
         << victimFrame + 1 << '\n';

    // Replace page 2 with page 4.
    if (manager.replacePage(4, victimFrame))
    {
        cout << "\nReplacement successful!\n";
        cout << "Page " << replacedPage
             << " replaced by Page 4.\n";
    }
    else
    {
        cout << "\nReplacement failed.\n";
        return 1;
    }

    cout << "\nAfter replacement:\n";

    for (const Frame& frame : manager.getFrames())
    {
        cout << "Frame " << frame.frameNumber + 1
             << " -> Page " << frame.pageNumber
             << '\n';
    }

    return 0;
}