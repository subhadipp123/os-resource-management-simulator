# OS Resource Management Simulator

A modular, interactive **Operating System Resource Management Simulator** built using **C++20**. The project demonstrates core operating system concepts through process management and CPU scheduling algorithms.

The simulator allows users to create and manage processes, execute scheduling algorithms, and analyze scheduling performance through execution timelines and Gantt charts.

## Implemented Modules

### 1. Process Management

Simulates basic process lifecycle management.

**Features:**
- Create processes with automatically assigned PIDs
- Display all processes
- Search processes by PID
- Change process states
- Terminate processes
- Validate process state transitions

**Supported Process States:**
- New
- Ready
- Running
- Waiting
- Terminated

### 2. CPU Scheduling

Implements five CPU scheduling algorithms.

| Algorithm | Type | Description |
|---|---|---|
| FCFS | Non-preemptive | First Come First Serve |
| SJF | Non-preemptive | Shortest Job First |
| SRTF | Preemptive | Shortest Remaining Time First |
| Priority Scheduling | Non-preemptive | Scheduling based on process priority |
| Priority Scheduling | Preemptive | Higher-priority processes can preempt running processes |

**Scheduling Features:**
- Process arrival and burst time input
- Process priority input where required
- CPU idle-time handling
- Process execution timeline
- Gantt chart generation
- Scheduling result calculation

**Performance Metrics:**
- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average Turnaround Time
- Average Waiting Time
- Average Response Time

**Priority Convention:** A lower numerical value represents a higher priority.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++20 | Core programming language |
| STL | Data structures and algorithms |
| OOP | Modular software design |
| GCC / MinGW | Compilation |
| Git | Version control |
| GitHub | Source code hosting |
| Visual Studio Code | Development environment |

## Project Structure

```text
OS-Resource-Management-Simulator/
│
├── main.cpp
├── menu.cpp
├── menu.h
│
├── process/
│   ├── Process.h
│   ├── Process.cpp
│   ├── ProcessManager.h
│   ├── ProcessManager.cpp
│   ├── ProcessManagementUI.h
│   └── ProcessManagementUI.cpp
│
├── scheduling/
│   ├── SchedulingProcess.h
│   ├── SchedulingOutcome.h
│   ├── SchedulingValidator.h
│   ├── SchedulingValidator.cpp
│   ├── FCFSScheduler.h
│   ├── FCFSScheduler.cpp
│   ├── SJFScheduler.h
│   ├── SJFScheduler.cpp
│   ├── SRTFScheduler.h
│   ├── SRTFScheduler.cpp
│   ├── PriorityScheduler1.h
│   ├── PriorityScheduler1.cpp
│   ├── PreemptivePriorityScheduler2.h
│   ├── PreemptivePriorityScheduler2.cpp
│   ├── GanttChart.h
│   ├── GanttChart.cpp
│   ├── CPUSchedulingUI.h
│   └── CPUSchedulingUI.cpp
│
├── output/
├── screenshots/
├── .gitignore
└── README.md
```

## Getting Started

### Prerequisites

- Windows 10 or Windows 11
- GCC compiler with C++20 support
- MinGW-w64
- Visual Studio Code or another C++ IDE
- PowerShell

### Clone the Repository

```bash
git clone https://github.com/subhadipp123/os-resource-management-simulator.git

cd os-resource-management-simulator
```

### Compile the Project

Run the following command from the project root directory:

```bash
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp menu.cpp process/Process.cpp process/ProcessManager.cpp process/ProcessManagementUI.cpp scheduling/FCFSScheduler.cpp scheduling/SJFScheduler.cpp scheduling/SRTFScheduler.cpp scheduling/PriorityScheduler1.cpp scheduling/PreemptivePriorityScheduler2.cpp scheduling/SchedulingValidator.cpp scheduling/GanttChart.cpp scheduling/CPUSchedulingUI.cpp -o output/simulator.exe
```

### Run the Simulator

```powershell
.\output\simulator.exe
```

## Testing

The implemented modules have been tested through individual algorithm tests and integrated menu-based execution.

Testing includes:

- Process creation and automatic PID assignment
- Process searching and termination
- Valid and invalid process-state transitions
- CPU scheduling with different arrival and burst times
- Processes arriving simultaneously
- CPU idle intervals
- Preemptive and non-preemptive scheduling
- Priority-based scheduling
- Scheduling metric calculations
- Gantt chart generation

## Future Enhancements

The following modules are planned but are not currently implemented:

- Memory Management
  - First Fit
  - Best Fit
  - Worst Fit
- Virtual Memory and Page Replacement
  - FIFO
  - LRU
  - Optimal
- Deadlock Management
  - Banker's Algorithm
  - Safe-state detection
- Additional performance analysis and benchmarking

## Learning Objectives

This project explores practical applications of:

- Operating System process management
- CPU scheduling algorithms
- Preemptive and non-preemptive scheduling
- Process state transitions
- Data structures and algorithms
- C++ modular programming
- Software design and testing

## Author

**Subhadip Pradhan**

GitHub: [subhadipp123](https://github.com/subhadipp123)
