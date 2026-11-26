
#include "PageTable.h"

#include <stdexcept>

PageTable::PageTable(int totalPages)
{
    if (totalPages <= 0)
    {
        throw std::invalid_argument(
            "Total pages must be greater than zero."
        );
    }

    entries.resize(totalPages);

    for (int i = 0; i < totalPages; i++)
    {
        entries[i].pageNumber = i;
        entries[i].frameNumber = -1;
        entries[i].valid = true;
        entries[i].present = false;
    }
}

bool PageTable::isValidPage(int pageNumber) const
{
    return pageNumber >= 0 &&
           pageNumber < static_cast<int>(entries.size()) &&
           entries[pageNumber].valid;
}

bool PageTable::isPagePresent(int pageNumber) const
{
    if (!isValidPage(pageNumber))
    {
        return false;
    }

    return entries[pageNumber].present;
}

int PageTable::getFrameNumber(int pageNumber) const
{
    if (!isValidPage(pageNumber) ||
        !entries[pageNumber].present)
    {
        return -1;
    }

    return entries[pageNumber].frameNumber;
}

bool PageTable::mapPage(int pageNumber, int frameNumber)
{
    if (!isValidPage(pageNumber) ||
        frameNumber < 0 ||
        entries[pageNumber].present)
    {
        return false;
    }

    entries[pageNumber].frameNumber = frameNumber;
    entries[pageNumber].present = true;

    return true;
}

bool PageTable::unmapPage(int pageNumber)
{
    if (!isValidPage(pageNumber) ||
        !entries[pageNumber].present)
    {
        return false;
    }

    entries[pageNumber].frameNumber = -1;
    entries[pageNumber].present = false;

    return true;
}

const PageTableEntry* PageTable::getEntry(int pageNumber) const
{
    if (!isValidPage(pageNumber))
    {
        return nullptr;
    }

    return &entries[pageNumber];
}

int PageTable::getTotalPages() const
{
    return static_cast<int>(entries.size());
}
