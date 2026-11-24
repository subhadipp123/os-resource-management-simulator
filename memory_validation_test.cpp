
#include <iostream>
#include <cassert>

#include "memory/MemoryManager.h"

int main()
{
    MemoryManager manager(1000);

    // Test 1: Valid allocation
    assert(manager.allocateMemory(
        1, 200, AllocationStrategy::FirstFit));

    // Test 2: Duplicate PID
    assert(!manager.allocateMemory(
        1, 100, AllocationStrategy::FirstFit));

    // Test 3: Zero-size allocation
    assert(!manager.allocateMemory(
        2, 0, AllocationStrategy::FirstFit));

    // Test 4: Negative-size allocation
    assert(!manager.allocateMemory(
        2, -100, AllocationStrategy::FirstFit));

    // Test 5: Invalid PID
    assert(!manager.allocateMemory(
        0, 100, AllocationStrategy::FirstFit));

    // Test 6: Allocation larger than available memory
    assert(!manager.allocateMemory(
        2, 1200, AllocationStrategy::FirstFit));

    // Test 7: Valid allocation
    assert(manager.allocateMemory(
        2, 300, AllocationStrategy::BestFit));

    // Test 8: Valid deallocation
    assert(manager.deallocateMemory(1));

    // Test 9: Deallocating the same PID again
    assert(!manager.deallocateMemory(1));

    // Test 10: Deallocating nonexistent PID
    assert(!manager.deallocateMemory(99));

    // Test 11: Invalid deallocation PID
    assert(!manager.deallocateMemory(0));

    // Test 12: Allocation after deallocation
    assert(manager.allocateMemory(
        3, 150, AllocationStrategy::WorstFit));

    // Test 13: Memory conservation
    assert(manager.getUsedMemory() +
           manager.getFreeMemory() ==
           manager.getTotalMemory());

    // Test 14: Release all remaining processes
    assert(manager.deallocateMemory(2));
    assert(manager.deallocateMemory(3));

    // Test 15: Complete memory recovery
    assert(manager.getFreeMemory() == 1000);
    assert(manager.getUsedMemory() == 0);
    assert(manager.getLargestFreeBlock() == 1000);

    std::cout << "All 15 memory validation tests passed.\n";

    return 0;
}
