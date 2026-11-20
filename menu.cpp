
#include <iostream>

#include "menu.h"
#include "process/ProcessManagementUI.h"
#include "scheduling/CPUSchedulingUI.h"

using namespace std;

// Displays the main menu.
void displayMenu()
{
    cout << "\n========================================\n";
    cout << "     OS RESOURCE MANAGEMENT SIMULATOR\n";
    cout << "========================================\n";

    cout << "\n1. Process Management\n";
    cout << "2. CPU Scheduling\n";
    cout << "3. Exit\n";
}

// Handles the user's choice.
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
            cout << "Exiting the simulator. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Please select 1 to 3.\n";
    }
}
