
#include <iostream>

#include "virtual_memory/Page.h"
#include "virtual_memory/Frame.h"
#include "virtual_memory/PageTableEntry.h"

using namespace std;

int main()
{
    // Create a virtual page
    Page page;
    page.pageNumber = 3;

    // Create a physical frame
    Frame frame;
    frame.frameNumber = 0;
    frame.pageNumber = 3;
    frame.occupied = true;
    frame.loadTime = 1;
    frame.lastUsedTime = 1;

    // Create a page table entry
    PageTableEntry entry;
    entry.pageNumber = 3;
    entry.frameNumber = 0;
    entry.valid = true;
    entry.present = true;

    // Display test results
    cout << "VIRTUAL MEMORY DATA STRUCTURES TEST\n";
    cout << "===================================\n";

    cout << "Page Number: "
         << page.pageNumber << '\n';

    cout << "Frame Number: "
         << frame.frameNumber << '\n';

    cout << "Stored Page: "
         << frame.pageNumber << '\n';

    cout << "Frame Occupied: "
         << (frame.occupied ? "Yes" : "No") << '\n';

    cout << "Page Table Mapping: "
         << "Page " << entry.pageNumber
         << " -> Frame " << entry.frameNumber << '\n';

    cout << "Page Valid: "
         << (entry.valid ? "Yes" : "No") << '\n';

    cout << "Page Present: "
         << (entry.present ? "Yes" : "No") << '\n';

    return 0;
}
