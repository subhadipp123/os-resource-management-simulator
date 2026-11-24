# OS Resource Management Simulator

A modular, interactive **Operating System Resource Management Simulator** developed using **C++20**. This project demonstrates fundamental operating system concepts through process management, CPU scheduling, performance analysis, and memory management.

The simulator allows users to manage process lifecycles, execute different CPU scheduling algorithms, visualize execution timelines using Gantt charts, compare scheduling performance, and simulate dynamic memory allocation using different allocation strategies.

## Table of Contents

- [Implemented Modules](#implemented-modules)
  - [1. Process Management](#1-process-management)
  - [2. CPU Scheduling](#2-cpu-scheduling)
  - [3. Performance Analysis](#3-performance-analysis)
  - [4. Memory Management](#4-memory-management)
- [Technology Stack](#technology-stack)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Main Menu](#main-menu)
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

**Important:** Scheduling performance metrics describe simulated CPU scheduling behavior, whereas execution time measures how long the C++ implementation takes to calculate a schedule on the host computer.

### 4. Memory Management

Simulates dynamic memory allocation and deallocation using variable-sized contiguous memory blocks.

**Supported Allocation Algorithms:**

| Algorithm | Description |
|---|---|
| First Fit | Selects the first free memory block large enough for the requested process. |
| Best Fit | Selects the smallest available free block that can accommodate the request. |
| Worst Fit | Selects the largest available free block that can accommodate the request. |

**Memory Management Features:**
- Initialize total memory capacity.
- Allocate memory to processes.
- Deallocate memory using Process IDs.
- Select First Fit, Best Fit, or Worst Fit allocation strategies.
- Split free memory blocks after allocation.
- Merge adjacent free blocks after deallocation (coalescing).
- Display the current memory map.
- Track allocated and free memory.
- Calculate memory utilization.
- Identify the largest available free block.
- Calculate external fragmentation.

**Memory Block Representation:**

Each memory block maintains:

| Attribute | Description |
|---|---|
| Start Address | Starting address of the memory block. |
| Size | Size of the memory block in KB. |
| Allocation Status | Indicates whether the block is free or allocated. |
| Process ID | Identifies the process occupying the block; free blocks use PID -1. |

**Memory Statistics:**

Memory Utilization:

Memory Utilization = (Used Memory / Total Memory) × 100

External Fragmentation:

External Fragmentation = Total Free Memory − Largest Free Block

External Fragmentation Percentage:

External Fragmentation % = (External Fragmentation / Total Free Memory) × 100

The external fragmentation percentage is defined as 0 when total free memory is zero.

**Memory Allocation Behavior:**
- A process receives one contiguous memory block.
- A selected free block is split when its size exceeds the requested allocation.
- Deallocated blocks are merged with adjacent free blocks.
- Duplicate active Process IDs and invalid allocation requests are rejected.
- Allocation fails when no individual free block is large enough.

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
├── memory/
│   ├── MemoryBlock.h
│   ├── MemoryManager.h
│   ├── MemoryManager.cpp
│   ├── MemoryAllocationUI.h
│   └── MemoryAllocationUI.cpp
│
├── output/
├── screenshots/
├── .gitignore
└── README.md
```

Additional standalone test files are maintained in the project root and module directories.

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
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp menu.cpp `
process/Process.cpp `
process/ProcessManager.cpp `
process/ProcessManagementUI.cpp `
scheduling/FCFSScheduler.cpp `
scheduling/SJFScheduler.cpp `
scheduling/SRTFScheduler.cpp `
scheduling/PriorityScheduler1.cpp `
scheduling/PreemptivePriorityScheduler2.cpp `
scheduling/SchedulingValidator.cpp `
scheduling/GanttChart.cpp `
scheduling/CPUSchedulingUI.cpp `
performance/SchedulingBenchmark.cpp `
performance/WorkloadGenerator.cpp `
performance/SchedulingReport.cpp `
performance/ManualInput.cpp `
performance/PerformanceAnalysisUI.cpp `
memory/MemoryManager.cpp `
memory/MemoryAllocationUI.cpp `
-o output/simulator.exe
```

**Note:** Standalone test source files containing their own `main()` functions should not be included in the application build command.

### Run the Simulator

```powershell
.\output\simulator.exe
```

## Main Menu

The simulator currently provides four implemented modules:

```text
========================================
     OS RESOURCE MANAGEMENT SIMULATOR
========================================

1. Process Management
2. CPU Scheduling
3. Performance Analysis
4. Memory Management
5. Exit
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

**Memory Management Testing:**
- Memory initialization and block representation.
- First Fit allocation and block splitting.
- Best Fit allocation and block selection.
- Worst Fit allocation and block selection.
- Memory deallocation.
- Adjacent free-block coalescing.
- Memory utilization calculations.
- External fragmentation calculations.
- Largest free block calculation.
- Duplicate PID validation.
- Invalid PID and allocation-size validation.
- Insufficient memory handling.
- Memory Management UI and main-menu integration.

## Future Enhancements

The following modules and improvements are planned but are not yet implemented.

### 1. Virtual Memory and Page Replacement

- Paging and virtual memory simulation.
- Page and frame management.
- Page table representation.
- Page fault handling.
- FIFO page replacement.
- LRU page replacement.
- Optimal page replacement.
- Page fault comparison and analysis.
- Belady's anomaly demonstration.

### 2. Deadlock Management

- Deadlock fundamentals and resource allocation.
- Resource Allocation Graph representation.
- Banker's Algorithm.
- Safe-state detection.
- Deadlock avoidance analysis.

### 3. Additional Enhancements

- Advanced performance visualizations.
- Expanded test coverage.
- Improved documentation.
- Screenshots and sample execution demonstrations.
- Further algorithm optimization.

## Learning Objectives

This project explores practical applications of:

- Operating System process management.
- Process lifecycle and state transitions.
- CPU scheduling algorithms.
- Preemptive and non-preemptive scheduling.
- Scheduling performance evaluation.
- Algorithm execution-time analysis.
- Contiguous memory allocation.
- Memory fragmentation and coalescing.
- Data structures and algorithms.
- C++20 programming.
- Modular software architecture.
- Software testing and debugging.

## Author

**Subhadip Pradhan**

GitHub: [subhadipp123](https://github.com/subhadipp123)
