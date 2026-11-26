
#include "VirtualMemoryManager.h"

#include <stdexcept>
#include <cstddef>

VirtualMemoryManager::VirtualMemoryManager(
    int totalPages,
    int totalFrames
)
    : pageTable(totalPages),
      frames(
          totalFrames > 0
              ? static_cast<std::size_t>(totalFrames)
              : throw std::invalid_argument(
                    "Total frames must be greater than zero."
                )
      ),
      logicalClock(0),
      pageFaults(0),
      pageHits(0)
{
    for (int i = 0; i < totalFrames; i++)
    {
        frames[i].frameNumber = i;
    }
}

int VirtualMemoryManager::findFreeFrame() const
{
    for (int i = 0; i < static_cast<int>(frames.size()); i++)
    {
        if (!frames[i].occupied)
            return i;
    }

    return -1;
}

PageAccessResult VirtualMemoryManager::accessPage(int pageNumber)
{
    if (!pageTable.isValidPage(pageNumber))
        return PageAccessResult::INVALID_PAGE;

    logicalClock++;

    if (pageTable.isPagePresent(pageNumber))
    {
        int frameNumber = pageTable.getFrameNumber(pageNumber);

        frames[frameNumber].lastUsedTime = logicalClock;

        pageHits++;

        return PageAccessResult::PAGE_HIT;
    }

    pageFaults++;

    int freeFrame = findFreeFrame();

    if (freeFrame == -1)
        return PageAccessResult::NO_FREE_FRAME;

    frames[freeFrame].pageNumber = pageNumber;
    frames[freeFrame].occupied = true;
    frames[freeFrame].loadTime = logicalClock;
    frames[freeFrame].lastUsedTime = logicalClock;

    if (!pageTable.mapPage(pageNumber, freeFrame))
    {
        frames[freeFrame] = Frame();
        frames[freeFrame].frameNumber = freeFrame;

        pageFaults--;

        return PageAccessResult::INVALID_PAGE;
    }

    return PageAccessResult::PAGE_FAULT_HANDLED;
}

bool VirtualMemoryManager::replacePage(
    int pageNumber,
    int victimFrame
)
{
    if (!pageTable.isValidPage(pageNumber) ||
        pageTable.isPagePresent(pageNumber))
    {
        return false;
    }

    if (victimFrame < 0 ||
        victimFrame >= static_cast<int>(frames.size()) ||
        !frames[victimFrame].occupied)
    {
        return false;
    }

    int oldPage = frames[victimFrame].pageNumber;

    if (!pageTable.isPagePresent(oldPage) ||
        pageTable.getFrameNumber(oldPage) != victimFrame)
    {
        return false;
    }

    if (!pageTable.unmapPage(oldPage))
        return false;

    if (!pageTable.mapPage(pageNumber, victimFrame))
    {
        pageTable.mapPage(oldPage, victimFrame);
        return false;
    }

    logicalClock++;

    frames[victimFrame].pageNumber = pageNumber;
    frames[victimFrame].loadTime = logicalClock;
    frames[victimFrame].lastUsedTime = logicalClock;

    return true;
}

bool VirtualMemoryManager::unloadPage(int pageNumber)
{
    if (!pageTable.isValidPage(pageNumber) ||
        !pageTable.isPagePresent(pageNumber))
    {
        return false;
    }

    int frameNumber = pageTable.getFrameNumber(pageNumber);

    if (frameNumber < 0 ||
        frameNumber >= static_cast<int>(frames.size()) ||
        !frames[frameNumber].occupied ||
        frames[frameNumber].pageNumber != pageNumber)
    {
        return false;
    }

    if (!pageTable.unmapPage(pageNumber))
        return false;

    frames[frameNumber].pageNumber = -1;
    frames[frameNumber].occupied = false;
    frames[frameNumber].loadTime = 0;
    frames[frameNumber].lastUsedTime = 0;

    return true;
}

int VirtualMemoryManager::getPageFaults() const
{
    return pageFaults;
}

int VirtualMemoryManager::getPageHits() const
{
    return pageHits;
}

double VirtualMemoryManager::getPageFaultRate() const
{
    int totalReferences = pageFaults + pageHits;

    if (totalReferences == 0)
        return 0.0;

    return static_cast<double>(pageFaults) /
           totalReferences * 100.0;
}

double VirtualMemoryManager::getPageHitRatio() const
{
    int totalReferences = pageFaults + pageHits;

    if (totalReferences == 0)
        return 0.0;

    return static_cast<double>(pageHits) /
           totalReferences * 100.0;
}

int VirtualMemoryManager::getTotalFrames() const
{
    return static_cast<int>(frames.size());
}

int VirtualMemoryManager::getTotalPages() const
{
    return pageTable.getTotalPages();
}

const std::vector<Frame>& VirtualMemoryManager::getFrames() const
{
    return frames;
}

const PageTable& VirtualMemoryManager::getPageTable() const
{
    return pageTable;
}
