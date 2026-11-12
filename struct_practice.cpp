
#include <iostream>
#include <string>

using namespace std;

struct Process
{
    int pid;
    string name;
    int priority;
};

int main()
{
    Process p1;

    p1.pid = 101;
    p1.name = "Chrome";
    p1.priority = 2;

    cout << "PID: " << p1.pid << endl;
    cout << "Name: " << p1.name << endl;
    cout << "Priority: " << p1.priority << endl;

    return 0;
}
