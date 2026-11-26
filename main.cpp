#include <iostream>
#include <limits>

#include "menu.h"

using namespace std;

int main()
{
    int choice = 0;

    do
    {
        displayMenu();

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                break;
            }

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout
                << "Invalid input! Please enter a number.\n";

            choice = 0;
        }
        else
        {
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            handleChoice(choice);
        }

    } while (choice != 6);

    return 0;
}