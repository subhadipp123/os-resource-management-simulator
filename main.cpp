
#include <iostream>
#include <limits>
#include "menu.h"

using namespace std;

int main()
{
    int choice;

    do
    {
        displayMenu();

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        // Remove the leftover newline after reading the menu choice.
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        handleChoice(choice);

    } while (choice != 7);

    return 0;
}
