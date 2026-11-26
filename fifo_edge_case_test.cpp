
#include <iostream>
#include <vector>

#include "virtual_memory/VirtualMemoryManager.h"
#include "virtual_memory/FIFOReplacement.h"

using namespace std;

void checkResult(const string& testName, bool passed)
{
    cout << testName << ": "
         << (passed ? "PASSED" : "FAILED") << '\n';
}

int main()
{
    cout << "FIFO EDGE CASE TESTING\n";
    cout << "======================\n\n";

    FIFOReplacement fifo;

    // TEST 1: All frames are empty.
    {
        vector<Frame> frames(3);

        for (int i = 0; i < 3; i++)
        {
            frames[i].frameNumber = i;
        }

        int victim = fifo.selectVictimFrame(frames);

        checkResult("Empty frames test", victim == -1);
    }

    // TEST 2: Only one frame is occupied.
    {
        vector<Frame> frames(3);

        for (int i = 0; i < 3; i++)
        {
            frames[i].frameNumber = i;
        }

        frames[1].occupied = true;
        frames[1].pageNumber = 5;
        frames[1].loadTime = 10;

        int victim = fifo.selectVictimFrame(frames);

        checkResult("Single occupied frame test", victim == 1);
    }

    // TEST 3: Select the oldest page, not the most recently used page.
    {
        vector<Frame> frames(3);

        frames[0].frameNumber = 0;
        frames[0].pageNumber = 4;
        frames[0].occupied = true;
        frames[0].loadTime = 5;
        frames[0].lastUsedTime = 20;

        frames[1].frameNumber = 1;
        frames[1].pageNumber = 7;
        frames[1].occupied = true;
        frames[1].loadTime = 8;
        frames[1].lastUsedTime = 9;

        frames[2].frameNumber = 2;
        frames[2].pageNumber = 9;
        frames[2].occupied = true;
        frames[2].loadTime = 12;
        frames[2].lastUsedTime = 14;

        int victim = fifo.selectVictimFrame(frames);

        checkResult("Oldest arrival test", victim == 0);
    }

    // TEST 4: Invalid page access.
    {
        VirtualMemoryManager manager(4, 2);

        PageAccessResult result = manager.accessPage(10);

        bool passed =
            result == PageAccessResult::INVALID_PAGE &&
            manager.getPageFaults() == 0 &&
            manager.getPageHits() == 0;

        checkResult("Invalid page access test", passed);
    }

    // TEST 5: FIFO order must not change after a page hit.
    {
        VirtualMemoryManager manager(4, 2);

        manager.accessPage(0);
        manager.accessPage(1);

        // Access Page 0 again.
        manager.accessPage(0);

        FIFOReplacement algorithm;

        int victim =
            algorithm.selectVictimFrame(manager.getFrames());

        checkResult("FIFO order unchanged after hit", victim == 0);
    }

    // TEST 6: Already-present page cannot be replaced.
    {
        VirtualMemoryManager manager(4, 2);

        manager.accessPage(0);
        manager.accessPage(1);

        int faultsBefore = manager.getPageFaults();

        bool result = manager.replacePage(0, 1);

        bool passed =
            !result &&
            manager.getPageTable().getFrameNumber(0) == 0 &&
            manager.getPageTable().getFrameNumber(1) == 1 &&
            manager.getPageFaults() == faultsBefore;

        checkResult("Already-present page replacement test", passed);
    }

    // TEST 7: Invalid victim frame must be rejected.
    {
        VirtualMemoryManager manager(4, 2);

        manager.accessPage(0);
        manager.accessPage(1);

        bool result = manager.replacePage(2, 5);

        bool passed =
            !result &&
            manager.getPageTable().getFrameNumber(0) == 0 &&
            manager.getPageTable().getFrameNumber(1) == 1;

        checkResult("Invalid victim frame test", passed);
    }

    // TEST 8: Successful replacement must update both mappings.
    {
        VirtualMemoryManager manager(4, 2);

        manager.accessPage(0);
        manager.accessPage(1);

        PageAccessResult result = manager.accessPage(2);

        bool replacementResult = manager.replacePage(2, 0);

        bool passed =
            result == PageAccessResult::NO_FREE_FRAME &&
            replacementResult &&
            !manager.getPageTable().isPagePresent(0) &&
            manager.getPageTable().getFrameNumber(2) == 0 &&
            manager.getFrames()[0].pageNumber == 2 &&
            manager.getPageFaults() == 3;

        checkResult("Successful replacement test", passed);
    }

    cout << "\nFIFO EDGE CASE TESTING COMPLETED\n";

    return 0;
}
