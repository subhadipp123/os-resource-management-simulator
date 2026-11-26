
#ifndef PAGE_TABLE_ENTRY_H
#define PAGE_TABLE_ENTRY_H

struct PageTableEntry
{
    int pageNumber;
    int frameNumber;
    bool valid;
    bool present;

    PageTableEntry()
        : pageNumber(-1),
          frameNumber(-1),
          valid(true),
          present(false)
    {
    }
};

#endif
