
#include <iostream>
#include "process/Process.h"

using namespace std;

int main()
{
    Process p1(1, "Chrome", 5, ProcessState::New);

    cout << "PID: " << p1.pid << endl;
    cout << "Name: " << p1.name << endl;
    cout << "Priority: " << p1.priority << endl;
    cout << "State: " << getStateName(p1.state) << endl;

    cout << "\n===== TRANSITION TESTS =====\n";

    cout << "New -> Ready: "
         << isValidTransition(ProcessState::New, ProcessState::Ready)
         << endl;

    cout << "New -> Waiting: "
         << isValidTransition(ProcessState::New, ProcessState::Waiting)
         << endl;

    cout << "Terminated -> Ready: "
         << isValidTransition(ProcessState::Terminated, ProcessState::Ready)
         << endl;

    return 0;
}
