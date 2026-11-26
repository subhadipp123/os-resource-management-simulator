
#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <vector>
#include "PageTableEntry.h"

class PageTable
{
private:
    std::vector<PageTableEntry> entries;

public:
    explicit PageTable(int totalPages);

    bool isValidPage(int pageNumber) const;

    bool isPagePresent(int pageNumber) const;

    int getFrameNumber(int pageNumber) const;

    bool mapPage(int pageNumber, int frameNumber);

    bool unmapPage(int pageNumber);

    const PageTableEntry* getEntry(int pageNumber) const;

    int getTotalPages() const;
};

#endif
