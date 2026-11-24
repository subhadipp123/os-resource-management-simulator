
#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

struct MemoryBlock
{
    int startAddress;
    int size;
    bool isFree;
    int processId;
};

#endif
