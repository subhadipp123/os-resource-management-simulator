
#include <iostream>
#include <limits>
using namespace std;

// Function declarations
void displayMenu();
void handleChoice(int choice);

int main()
{
    int choice = 0;

    do
    {
        displayMenu();

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Please enter a number.\n";
            choice = 0;
        }
        else
        {
            handleChoice(choice);
        }

    } while (choice != 6);

    return 0;
}

// Displays the main menu
void displayMenu()
{
    cout << "\n========================================\n";
    cout << "     OS RESOURCE MANAGEMENT SIMULATOR\n";
    cout << "========================================\n";

    cout << "\n1. Process Management\n";
    cout << "2. CPU Scheduling\n";
    cout << "3. Memory Management\n";
    cout << "4. Virtual Memory & Page Replacement\n";
    cout << "5. Deadlock Management\n";
    cout << "6. Exit\n";
}

// Handles the user's choice
void handleChoice(int choice)
{
    switch (choice)
    {
        case 1:
            cout << "Process Management selected.\n";
            break;

        case 2:
            cout << "CPU Scheduling selected.\n";
            break;

        case 3:
            cout << "Memory Management selected.\n";
            break;

        case 4:
            cout << "Virtual Memory selected.\n";
            break;

        case 5:
            cout << "Deadlock Management selected.\n";
            break;

        case 6:
            cout << "Exiting the simulator. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Please select 1 to 6.\n";
    }
}
