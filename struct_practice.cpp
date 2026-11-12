#include <iostream>
#include <string>

using namespace std;

enum class ProcessState
{
    New,
    Ready,
    Running,
    Waiting,
    Terminated
};

struct Process
{
    int pid;
    string name;
    int priority;
    ProcessState state;
};

int main()
{
    Process p1;

    p1.pid = 101;
    p1.name = "Chrome";
    p1.priority = 2;
    p1.state = ProcessState::Ready;

    cout << "PID: " << p1.pid << endl;
    cout << "Name: " << p1.name << endl;
    cout << "Priority: " << p1.priority << endl;

    return 0;
}