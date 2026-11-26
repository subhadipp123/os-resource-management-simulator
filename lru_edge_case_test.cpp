
#include <iostream>
#include <vector>

#include "virtual_memory/VirtualMemoryManager.h"
#include "virtual_memory/LRUReplacement.h"

using namespace std;

int main()
{
    LRUReplacement lru;

    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const string& name)
    {
        if (condition)
        {
            cout << "[PASS] " << name << '\n';
            passed++;
        }
        else
        {
            cout << "[FAIL] " << name << '\n';
            failed++;
        }
    };

    // Test 1: All frames empty.
    vector<Frame> emptyFrames(3);

    check(
        lru.selectVictimFrame(emptyFrames) == -1,
        "Empty frames return -1"
    );

    // Test 2: Only one occupied frame.
    vector<Frame> singleFrame(3);

    singleFrame[1].occupied = true;
    singleFrame[1].pageNumber = 5;
    singleFrame[1].lastUsedTime = 10;

    check(
        lru.selectVictimFrame(singleFrame) == 1,
        "Select the only occupied frame"
    );

    // Test 3: Select least recently used page.
    VirtualMemoryManager manager(6, 3);

    manager.accessPage(1);
    manager.accessPage(2);
    manager.accessPage(3);

    // Page 1 becomes recently used.
    manager.accessPage(1);

    check(
        lru.selectVictimFrame(manager.getFrames()) == 1,
        "Select Page 2 as the least recently used"
    );

    // Test 4: Access Page 2 and check recency changes.
    manager.accessPage(2);

    check(
        lru.selectVictimFrame(manager.getFrames()) == 2,
        "Page hit updates LRU order"
    );

    // Test 5: Invalid page does not change statistics.
    int faultsBefore = manager.getPageFaults();
    int hitsBefore = manager.getPageHits();

    PageAccessResult invalidResult =
        manager.accessPage(10);

    check(
        invalidResult == PageAccessResult::INVALID_PAGE &&
        manager.getPageFaults() == faultsBefore &&
        manager.getPageHits() == hitsBefore,
        "Invalid page does not change statistics"
    );

    // Test 6: Invalid victim frame is rejected.
    check(
        !manager.replacePage(4, -1),
        "Negative victim frame is rejected"
    );

    // Test 7: Out-of-range victim frame is rejected.
    check(
        !manager.replacePage(4, 10),
        "Out-of-range victim frame is rejected"
    );

    // Test 8: Successful LRU replacement.
    int victimFrame =
        lru.selectVictimFrame(manager.getFrames());

    int victimPage =
        manager.getFrames()[victimFrame].pageNumber;

    bool replaced =
        manager.replacePage(4, victimFrame);

    check(
        replaced &&
        manager.getPageTable().isPagePresent(4) &&
        !manager.getPageTable().isPagePresent(victimPage),
        "Replacement updates page table mappings"
    );

    cout << "\n============================\n";
    cout << "LRU EDGE CASE TEST SUMMARY\n";
    cout << "============================\n";

    cout << "Passed: " << passed << '\n';
    cout << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
