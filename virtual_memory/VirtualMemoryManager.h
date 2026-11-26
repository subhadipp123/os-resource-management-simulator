
#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include <vector>

#include "Frame.h"
#include "PageTable.h"

enum class PageAccessResult
{
    INVALID_PAGE,
    PAGE_HIT,
    PAGE_FAULT_HANDLED,
    NO_FREE_FRAME
};

class VirtualMemoryManager
{
private:
    PageTable pageTable;
    std::vector<Frame> frames;

    unsigned long long logicalClock;
    int pageFaults;
    int pageHits;

    int findFreeFrame() const;

public:
    VirtualMemoryManager(int totalPages, int totalFrames);

    PageAccessResult accessPage(int pageNumber);

    bool replacePage(int pageNumber, int victimFrame);

    bool unloadPage(int pageNumber);

    int getPageFaults() const;
    int getPageHits() const;

    double getPageFaultRate() const;
    double getPageHitRatio() const;

    int getTotalFrames() const;
    int getTotalPages() const;

    const std::vector<Frame>& getFrames() const;
    const PageTable& getPageTable() const;
};

#endif
