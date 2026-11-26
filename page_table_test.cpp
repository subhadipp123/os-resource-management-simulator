
#include <iostream>
#include "virtual_memory/PageTable.h"

using namespace std;

int main()
{
    cout << "PAGE TABLE MANAGEMENT TEST\n";
    cout << "==========================\n\n";

    PageTable pageTable(5);

    cout << "Total Virtual Pages: "
         << pageTable.getTotalPages() << "\n\n";

    // Map virtual pages to physical frames
    cout << "Mapping Pages:\n";

    cout << "Page 0 -> Frame 2: "
         << (pageTable.mapPage(0, 2) ? "Success" : "Failed")
         << '\n';

    cout << "Page 2 -> Frame 0: "
         << (pageTable.mapPage(2, 0) ? "Success" : "Failed")
         << '\n';

    cout << "Page 4 -> Frame 3: "
         << (pageTable.mapPage(4, 3) ? "Success" : "Failed")
         << "\n\n";

    // Check page presence
    cout << "Page Presence:\n";

    for (int i = 0; i < pageTable.getTotalPages(); i++)
    {
        cout << "Page " << i << ": "
             << (pageTable.isPagePresent(i) ? "Present" : "Not Present")
             << '\n';
    }

    // Retrieve physical frame
    cout << "\nFrame Mapping:\n";

    cout << "Page 0 -> Frame "
         << pageTable.getFrameNumber(0) << '\n';

    cout << "Page 2 -> Frame "
         << pageTable.getFrameNumber(2) << '\n';

    cout << "Page 1 -> Frame "
         << pageTable.getFrameNumber(1) << '\n';

    // Test invalid page
    cout << "\nInvalid Page Test:\n";

    cout << "Page 7 Valid: "
         << (pageTable.isValidPage(7) ? "Yes" : "No")
         << '\n';

    // Unmap a page
    cout << "\nUnmapping Page 2:\n";

    cout << (pageTable.unmapPage(2) ? "Success" : "Failed")
         << '\n';

    cout << "Page 2 Present: "
         << (pageTable.isPagePresent(2) ? "Yes" : "No")
         << '\n';

    cout << "Page 2 Frame: "
         << pageTable.getFrameNumber(2) << '\n';

    return 0;
}
