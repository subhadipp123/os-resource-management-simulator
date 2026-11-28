
#include <iostream>
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
            cin.ignore(10000, '\n');

            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        handleChoice(choice);

    } while (choice != 7);

    return 0;
}
