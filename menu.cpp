
#include <iostream>

#include "menu.h"
#include "process/ProcessManagementUI.h"
#include "scheduling/CPUSchedulingUI.h"
#include "performance/PerformanceAnalysisUI.h"
#include "memory/MemoryAllocationUI.h"

using namespace std;

void displayMenu()
{
    cout << "\n========================================\n";
    cout << "     OS RESOURCE MANAGEMENT SIMULATOR\n";
    cout << "========================================\n";

    cout << "\n1. Process Management\n";
    cout << "2. CPU Scheduling\n";
    cout << "3. Performance Analysis\n";
    cout << "4. Memory Management\n";
    cout << "5. Exit\n";
}

void handleChoice(int choice)
{
    switch (choice)
    {
        case 1:
            processManagementMenu();
            break;

        case 2:
            cpuSchedulingMenu();
            break;

        case 3:
            performanceAnalysisMenu();
            break;

        case 4:
            runMemoryManagement();
            break;

        case 5:
            cout << "Exiting the simulator. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Please select 1 to 5.\n";
    }
}
