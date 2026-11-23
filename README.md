# OS Resource Management Simulator

A modular, interactive **Operating System Resource Management Simulator** developed using **C++20**. This project demonstrates fundamental operating system concepts through process management, CPU scheduling, and performance analysis.

The simulator allows users to manage process lifecycles, execute different CPU scheduling algorithms, visualize execution timelines using Gantt charts, and compare scheduling performance using manual inputs or automatically generated workloads.

## Table of Contents

- [Implemented Modules](#implemented-modules)
  - [1. Process Management](#1-process-management)
  - [2. CPU Scheduling](#2-cpu-scheduling)
  - [3. Performance Analysis](#3-performance-analysis)
- [Technology Stack](#technology-stack)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Testing](#testing)
- [Future Enhancements](#future-enhancements)
- [Learning Objectives](#learning-objectives)
- [Author](#author)

---

## Implemented Modules

### 1. Process Management

Simulates basic process lifecycle management.

**Features:**
- Create processes with automatically assigned PIDs.
- Display all processes.
- Search processes by PID.
- Change process states.
- Terminate processes.
- Validate process state transitions.

**Supported Process States:**

| State | Description |
|---|---|
| New | Process has been created. |
| Ready | Process is waiting for CPU allocation. |
| Running | Process is currently executing. |
| Waiting | Process is waiting for an event or resource. |
| Terminated | Process has completed or been terminated. |

**Supported Process State Transitions:**
- New → Ready
- Ready → Running
- Running → Ready
- Running → Waiting
- Waiting → Ready
- Any non-terminated state → Terminated

Terminated processes cannot transition to another state.

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
- Process arrival time and burst time input.
- Process priority input where required.
- CPU idle-time handling.
- Preemptive and non-preemptive scheduling.
- Process execution timeline generation.
- Gantt chart visualization.
- Scheduling result calculation.
- Input validation.

**Priority Convention:**

A lower numerical value represents a higher priority.

**Scheduling Metrics:**

| Metric | Description |
|---|---|
| Completion Time (CT) | Time at which a process finishes execution. |
| Turnaround Time (TAT) | Total time from process arrival to completion. |
| Waiting Time (WT) | Total time spent waiting in the ready queue. |
| Response Time (RT) | Time from arrival until the process first gets CPU execution. |
| Average Waiting Time | Average waiting time across all processes. |
| Average Turnaround Time | Average turnaround time across all processes. |
| Average Response Time | Average response time across all processes. |

### 3. Performance Analysis

Provides two modes for comparing the five CPU scheduling algorithms.

#### A. Manual Input Comparison

Allows users to enter process details manually and compare all five scheduling algorithms using identical process inputs.

**User Inputs:**
- Number of processes (1–100).
- Arrival Time (AT).
- Burst Time (BT).
- Priority.

Process IDs are automatically assigned.

**Comparison Metrics:**
- Average Waiting Time.
- Average Turnaround Time.
- Average Response Time.
- CPU Utilization.
- Throughput.

The same process workload is passed to all five scheduling algorithms to ensure a fair comparison.

#### B. Automatic Benchmark

Automatically generates process workloads and evaluates the performance of all five scheduling algorithms.

**Configurable Parameters:**

| Parameter | Supported Range |
|---|---|
| Number of processes | 1–1,000 |
| Repetitions | 1–30 |
| Random seed | 0–2,147,483,647 |

**Workload Generation:**

The simulator uses the C++ `<random>` library and `std::mt19937` to generate reproducible process workloads.

Generated process attributes include:
- Unique Process ID (PID).
- Arrival Time: 0–100.
- Burst Time: 1–20.
- Priority: 0–9.

Using the same random seed and process count produces the same workload.

**Performance Metrics:**

| Metric | Description |
|---|---|
| Average Waiting Time | Average time processes spend waiting. |
| Average Turnaround Time | Average time from arrival to completion. |
| Average Response Time | Average time until first CPU execution. |
| CPU Utilization | Percentage of simulated time the CPU is busy. |
| Throughput | Number of completed processes per simulated time unit. |
| Median Execution Time | Median actual scheduler computation time. |
| Minimum Execution Time | Lowest measured scheduler computation time. |
| Maximum Execution Time | Highest measured scheduler computation time. |

**Benchmark Measurement Methodology:**

- All five algorithms receive the same generated process workload.
- Each algorithm is executed repeatedly according to the configured repetition count.
- `std::chrono::steady_clock` is used to measure actual scheduler execution time.
- Median execution time is used as the primary computational performance measurement because it is less sensitive to unusually slow measurements.
- Workload generation and report printing are excluded from scheduler execution-time measurements.
- Scheduler execution time includes internal validation and scheduling calculations.

**Metric Calculations:**

CPU Utilization:

CPU Utilization = (Total Burst Time / Simulated Makespan) × 100

Throughput:

Throughput = Completed Processes / Simulated Makespan

The simulated makespan is measured from time zero to the final process completion time, including any initial CPU idle interval.

**Important:** Scheduling performance metrics describe the simulated CPU scheduling behavior, whereas execution time measures how long the C++ implementation takes to calculate a schedule on the host computer.

---

## Technology Stack

| Technology | Purpose |
|---|---|
| C++20 | Core programming language |
| Standard Template Library (STL) | Data structures and algorithms |
| Object-Oriented Programming | Modular software design |
| `<chrono>` | Execution-time measurement |
| `<random>` | Reproducible workload generation |
| GCC / MinGW-w64 | Compilation |
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
├── performance/
│   ├── SchedulingBenchmark.h
│   ├── SchedulingBenchmark.cpp
│   ├── WorkloadGenerator.h
│   ├── WorkloadGenerator.cpp
│   ├── SchedulingReport.h
│   ├── SchedulingReport.cpp
│   ├── ManualInput.h
│   ├── ManualInput.cpp
│   ├── PerformanceAnalysisUI.h
│   └── PerformanceAnalysisUI.cpp
│
├── output/
├── screenshots/
├── .gitignore
└── README.md
```

## Getting Started

### Prerequisites

- Windows 10 or Windows 11.
- GCC compiler with C++20 support.
- MinGW-w64.
- Visual Studio Code or another C++ IDE.
- PowerShell.

### Clone the Repository

```bash
git clone https://github.com/subhadipp123/os-resource-management-simulator.git

cd os-resource-management-simulator
```

### Create the Output Directory

If the output directory does not already exist:

```powershell
mkdir output
```

### Compile the Project

Run the following command from the project root directory:

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp menu.cpp process/Process.cpp process/ProcessManager.cpp process/ProcessManagementUI.cpp scheduling/FCFSScheduler.cpp scheduling/SJFScheduler.cpp scheduling/SRTFScheduler.cpp scheduling/PriorityScheduler1.cpp scheduling/PreemptivePriorityScheduler2.cpp scheduling/SchedulingValidator.cpp scheduling/GanttChart.cpp scheduling/CPUSchedulingUI.cpp performance/SchedulingBenchmark.cpp performance/WorkloadGenerator.cpp performance/SchedulingReport.cpp performance/ManualInput.cpp performance/PerformanceAnalysisUI.cpp -o output/simulator.exe
```

### Run the Simulator

```powershell
.\output\simulator.exe
```

## Main Menu

The simulator currently provides three implemented modules:

```text
========================================
     OS RESOURCE MANAGEMENT SIMULATOR
========================================

1. Process Management
2. CPU Scheduling
3. Performance Analysis
4. Exit
```

## Testing

The implemented modules have been tested through individual component tests and integrated menu-based execution.

**Process Management Testing:**
- Process creation and automatic PID assignment.
- Process searching and termination.
- Valid and invalid process-state transitions.
- Process state management.

**CPU Scheduling Testing:**
- Different process arrival and burst times.
- Simultaneous process arrivals.
- CPU idle intervals.
- Preemptive and non-preemptive scheduling.
- Priority-based scheduling.
- Scheduling metric calculations.
- Gantt chart generation.

**Performance Analysis Testing:**
- Manual process input and validation.
- Comparison using identical process workloads.
- Automatic workload generation.
- Reproducible workload generation using random seeds.
- Benchmarking with different process counts.
- Repeated execution-time measurements.
- Median, minimum and maximum execution-time calculations.
- Comparison report generation.
- Integrated menu navigation.

## Future Enhancements

The following modules are planned but are not currently implemented:

### 1. Memory Management
- First Fit
- Best Fit
- Worst Fit
- Memory allocation and deallocation.
- Memory fragmentation analysis.

### 2. Virtual Memory and Page Replacement
- FIFO
- LRU
- Optimal Page Replacement
- Page fault analysis.

### 3. Deadlock Management
- Banker's Algorithm.
- Safe-state detection.
- Resource allocation and request management.
- Deadlock avoidance analysis.

### 4. Additional Enhancements
- Advanced performance visualizations.
- Further algorithm optimization.
- Expanded test coverage.
- Additional system performance analysis.

## Learning Objectives

This project explores practical applications of:

- Operating System process management.
- Process lifecycle and state transitions.
- CPU scheduling algorithms.
- Preemptive and non-preemptive scheduling.
- Scheduling performance evaluation.
- Algorithm execution-time analysis.
- Data structures and algorithms.
- C++20 programming.
- Modular software architecture.
- Software testing and debugging.

## Author

**Subhadip Pradhan**

GitHub: [subhadipp123](https://github.com/subhadipp123)