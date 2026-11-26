# OS Resource Management Simulator

A modular, interactive **Operating System Resource Management Simulator** built with **C++20**. The project demonstrates core operating-system resource-management concepts through process lifecycle management, CPU scheduling, performance benchmarking, contiguous memory allocation, and virtual-memory page replacement.

The application is a command-line simulator: it models OS algorithms using controlled inputs and reports their behavior. It does **not** manage real Windows/Linux processes, control physical memory, or implement a kernel.

**Repository:** https://github.com/subhadipp123/os-resource-management-simulator

---

## Table of Contents

- [Project Overview](#project-overview)
- [Implemented Modules](#implemented-modules)
  - [1. Process Management](#1-process-management)
  - [2. CPU Scheduling](#2-cpu-scheduling)
  - [3. Performance Analysis](#3-performance-analysis)
  - [4. Memory Management](#4-memory-management)
  - [5. Virtual Memory and Page Replacement](#5-virtual-memory-and-page-replacement)
- [Technology Stack](#technology-stack)
- [Project Architecture](#project-architecture)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Build and Run](#build-and-run)
- [Main Menu](#main-menu)
- [Testing and Validation](#testing-and-validation)
- [Key Concepts Demonstrated](#key-concepts-demonstrated)
- [Current Scope and Limitations](#current-scope-and-limitations)
- [Planned Enhancements](#planned-enhancements)
- [Learning Outcomes](#learning-outcomes)
- [Author](#author)

---

## Project Overview

Operating systems manage finite resources such as CPU time and main memory while multiple processes compete for them. Different scheduling, allocation, and replacement policies can produce different outcomes depending on the workload.

This project provides an interactive environment to:

- Create and manage simulated processes and their lifecycle states.
- Run multiple CPU scheduling algorithms on process workloads.
- Inspect execution order and Gantt-chart timelines.
- Compare scheduling behavior using calculated metrics.
- Benchmark scheduler implementation time using repeatable workloads.
- Allocate and release contiguous memory blocks.
- Observe memory splitting, coalescing, utilization, and external fragmentation.
- Simulate virtual page references across a fixed number of physical frames.
- Compare FIFO, LRU, and Optimal page replacement.
- Inspect page hits, page faults, frame contents, and replacement decisions.
- Demonstrate Belady's Anomaly with FIFO page replacement.

The code is organized into modules so that algorithm logic, data structures, user interaction, and reporting can be developed and tested separately.

---

## Implemented Modules

### 1. Process Management

Simulates basic process lifecycle management.

**Features**

- Create processes with automatically assigned Process IDs (PIDs).
- Display the process table.
- Search for a process using its PID.
- Change process states.
- Terminate processes.
- Validate process state transitions.

**Supported process states**

| State | Description |
|---|---|
| New | The process has been created. |
| Ready | The process is ready and waiting for CPU allocation. |
| Running | The process is currently executing. |
| Waiting | The process is waiting for an event or resource. |
| Terminated | The process has completed or has been terminated. |

**Supported state transitions**

- New → Ready
- Ready → Running
- Running → Ready
- Running → Waiting
- Waiting → Ready
- Any non-terminated state → Terminated

A terminated process cannot transition to another state.

### 2. CPU Scheduling

Implements five CPU scheduling algorithms.

| Algorithm | Scheduling type | Description |
|---|---|---|
| First Come First Serve (FCFS) | Non-preemptive | Processes are selected in arrival order. |
| Shortest Job First (SJF) | Non-preemptive | Selects the available process with the shortest burst time. |
| Shortest Remaining Time First (SRTF) | Preemptive | Selects the available process with the shortest remaining execution time. |
| Priority Scheduling | Non-preemptive | Selects an available process based on priority. |
| Priority Scheduling | Preemptive | A higher-priority process can preempt the currently running process. |

**Features**

- Accepts process arrival time and burst time.
- Accepts priority where required.
- Handles CPU idle intervals.
- Supports preemptive and non-preemptive scheduling.
- Generates process execution timelines.
- Displays text-based Gantt charts.
- Calculates per-process and aggregate scheduling metrics.
- Validates scheduling inputs.

**Priority convention:** A lower numerical value represents a higher priority.

**Scheduling metrics**

| Metric | Meaning |
|---|---|
| Completion Time (CT) | Time at which a process finishes execution. |
| Turnaround Time (TAT) | Total time from process arrival to completion. |
| Waiting Time (WT) | Total time spent waiting in the ready queue. |
| Response Time (RT) | Time from arrival until the process first receives CPU time. |
| Average Waiting Time | Average waiting time across all processes. |
| Average Turnaround Time | Average turnaround time across all processes. |
| Average Response Time | Average response time across all processes. |

### 3. Performance Analysis

Provides two ways to compare the five CPU scheduling algorithms.

#### A. Manual workload comparison

Users enter a process workload, and the simulator runs all five scheduling algorithms against the same input.

**Inputs**

- Number of processes (1–100).
- Arrival time.
- Burst time.
- Priority.
- Process IDs are assigned automatically.

**Comparison metrics**

- Average waiting time.
- Average turnaround time.
- Average response time.
- CPU utilization.
- Throughput.

Using the same workload for each scheduler makes the comparison consistent.

#### B. Automatic benchmark

Generates process workloads and measures scheduler performance over repeated runs.

**Configurable parameters**

| Parameter | Supported range |
|---|---:|
| Number of processes | 1–1,000 |
| Repetitions | 1–30 |
| Random seed | 0–2,147,483,647 |

The workload generator uses the C++ `<random>` library and `std::mt19937`.

Generated process attributes include:

- Unique Process ID.
- Arrival time: 0–100.
- Burst time: 1–20.
- Priority: 0–9.

The same process count and random seed reproduce the same workload.

**Benchmark metrics**

| Metric | Meaning |
|---|---|
| Average Waiting Time | Average time spent waiting for CPU execution. |
| Average Turnaround Time | Average time from arrival to completion. |
| Average Response Time | Average time until first CPU execution. |
| CPU Utilization | Percentage of simulated time for which the CPU is busy. |
| Throughput | Number of completed processes per simulated time unit. |
| Median Execution Time | Median measured scheduler computation time. |
| Minimum Execution Time | Lowest measured scheduler computation time. |
| Maximum Execution Time | Highest measured scheduler computation time. |

**Measurement methodology**

- All five schedulers receive the same generated workload.
- Each scheduler is executed repeatedly according to the configured repetition count.
- `std::chrono::steady_clock` measures scheduler computation time.
- Median execution time is used as the primary computational timing statistic because it is less sensitive to unusually slow measurements.
- Workload generation and report printing are excluded from scheduler execution-time measurements.
- Scheduler execution time includes internal validation and scheduling calculations.

**Metric calculations**

```text
CPU Utilization = (Total Burst Time / Simulated Makespan) × 100

Throughput = Completed Processes / Simulated Makespan
```

The simulated makespan is measured from time zero to the final process completion time, including any initial CPU idle interval.

**Important distinction:** Scheduling metrics describe the simulated behavior of a CPU scheduling policy. Benchmark execution time measures how long the C++ implementation takes to calculate the schedule on the host computer. These are different measurements.

### 4. Memory Management

Simulates dynamic allocation and deallocation using variable-sized contiguous memory blocks.

**Supported allocation algorithms**

| Algorithm | Selection rule |
|---|---|
| First Fit | Selects the first free block large enough for the request. |
| Best Fit | Selects the smallest free block that can accommodate the request. |
| Worst Fit | Selects the largest available free block. |

**Features**

- Initialize total memory capacity.
- Allocate memory to processes.
- Deallocate memory using Process IDs.
- Select First Fit, Best Fit, or Worst Fit.
- Split a free block when only part of it is allocated.
- Merge adjacent free blocks after deallocation (coalescing).
- Display the current memory map.
- Track allocated and free memory.
- Calculate memory utilization.
- Identify the largest available free block.
- Calculate external fragmentation.
- Reject duplicate active PIDs and invalid allocation requests.

**Memory block representation**

| Attribute | Description |
|---|---|
| Start Address | Starting address of the memory block. |
| Size | Size of the memory block in KB. |
| Allocation Status | Indicates whether the block is free or allocated. |
| Process ID | Identifies the process occupying the block; free blocks use PID `-1`. |

**Memory statistics**

```text
Memory Utilization =
    (Used Memory / Total Memory) × 100

External Fragmentation =
    Total Free Memory - Largest Free Block

External Fragmentation Percentage =
    (External Fragmentation / Total Free Memory) × 100
```

The external fragmentation percentage is defined as zero when total free memory is zero.

**Allocation behavior**

- A process receives one contiguous memory block.
- A selected free block is split when its size exceeds the requested allocation.
- Deallocated blocks are merged with adjacent free blocks.
- Allocation fails when no individual free block is large enough, even if the sum of all free blocks would be sufficient.
- Invalid sizes, invalid PIDs, and duplicate active PIDs are rejected.

### 5. Virtual Memory and Page Replacement

Simulates demand paging and page replacement using a page-reference string and a fixed number of page frames.

The virtual-memory module maintains page-table entries and frame information, handles page hits and page faults, and runs replacement algorithms when a page must be loaded but no free frame is available.

**Implemented algorithms**

| Algorithm | Replacement rule |
|---|---|
| FIFO (First-In, First-Out) | Replaces the page that has been in memory for the longest time. |
| LRU (Least Recently Used) | Replaces the page that has not been used for the longest time. |
| Optimal (OPT) | Replaces the page whose next use is farthest in the future, or which will not be used again. |

**Virtual-memory features**

- Accept a page-reference string.
- Accept a positive number of page frames.
- Maintain page-table entries.
- Track page-to-frame mappings.
- Distinguish valid pages from pages currently resident in memory.
- Detect page hits.
- Detect page faults.
- Load pages into free frames.
- Replace pages when all frames are occupied.
- Unload pages and release frames.
- Track page hits and page faults.
- Calculate hit ratio and page-fault rate.
- Display frame contents after each reference.
- Display whether each reference was a HIT or FAULT.
- Compare FIFO, LRU, and Optimal using the same reference string.
- Check for Belady's Anomaly at two consecutive frame counts.

**Core data structures**

| Component | Responsibility |
|---|---|
| `Page` | Represents a simulated page. |
| `Frame` | Stores a frame number, resident page, occupancy state, load time, and last-used time. |
| `PageTableEntry` | Stores page number, frame mapping, validity, and presence information. |
| `PageTable` | Maintains page-table entries and supports mapping and unmapping. |
| `VirtualMemoryManager` | Handles page access, page hits, page faults, frame allocation, replacement mapping, and statistics. |
| `PageReferenceMapper` | Maps original page numbers to compact internal IDs for efficient simulation. |

**Page-table state**

The page-table entry distinguishes:

- Valid and present: the page is valid and currently loaded in a frame.
- Valid and not present: the page is valid but is not currently resident.
- Invalid: the page number is outside the process's valid page range.

**Page replacement simulation reports**

Each simulation records:

- The page reference being processed.
- Frame contents after the reference.
- Whether the reference was a hit or fault.
- The replaced page, when applicable.
- The target frame.
- Total hits and faults.
- Hit ratio and page-fault rate.

**Page statistics**

```text
Hit Ratio (%) =
    (Page Hits / Total Page References) × 100

Page-Fault Rate (%) =
    (Page Faults / Total Page References) × 100
```

If there are no page references, the reported ratios are zero.

**Page-number scalability**

The simulators use a compact mapping for the distinct page numbers in a reference string. This avoids creating a page table whose size is based on the largest numeric page value.

For example, a reference string containing page numbers `10`, `500000`, and `2000000` can be simulated without allocating millions of page-table entries.

Negative page numbers and empty reference strings are rejected. The number of frames must be greater than zero.

**Belady's Anomaly checker**

The UI can compare page faults for a selected algorithm using `N` frames and `N + 1` frames.

Belady's Anomaly occurs when increasing the number of frames causes the number of page faults to increase. FIFO can exhibit this behavior.

Example reference string:

```text
1 2 3 4 1 2 5 1 2 3 4 5
```

With FIFO:

| Number of frames | Page faults |
|---:|---:|
| 3 | 9 |
| 4 | 10 |

This input demonstrates Belady's Anomaly because the fault count increases from 9 to 10 when the frame count increases from 3 to 4.

The checker evaluates only the selected reference string and the selected pair of consecutive frame counts. A result saying that an anomaly was not observed does not prove that the algorithm can never exhibit one.

**Optimal algorithm note:** Optimal replacement uses knowledge of future references. It is useful as a theoretical benchmark, but is not generally implementable as an online replacement policy when future references are unknown.

---

## Technology Stack

| Technology | Purpose |
|---|---|
| C++20 | Core programming language |
| Standard Template Library (STL) | Vectors, containers, algorithms, and supporting data structures |
| Object-Oriented Programming | Modular software design |
| `<chrono>` | Scheduler execution-time measurement |
| `<random>` | Reproducible workload generation |
| `<unordered_map>` | Compact page-reference mapping |
| GCC / MinGW-w64 | Compilation |
| PowerShell | Build and run commands on Windows |
| Visual Studio Code | Development environment |
| Git | Version control |
| GitHub | Source code hosting |

---

## Project Architecture

The project follows a modular organization. The main menu routes the user's selection to a module-specific UI. The UI gathers and validates input, invokes the module logic, and displays the resulting report.

```text
                         User
                          |
                          v
                    Main Menu (CLI)
                          |
          +---------------+----------------+
          |               |                |
          v               v                v
    Process UI       Scheduling UI    Performance UI
          |               |                |
          v               v                v
    ProcessManager    Scheduling       Benchmarking
                          |
                          v
                    Gantt / Metrics

          +----------------------+----------------------+
          |                                             |
          v                                             v
    Memory Management UI                     Virtual Memory UI
          |                                             |
          v                                             v
     MemoryManager                         Page Replacement Simulators
                                                        |
                                  +---------------------+------------------+
                                  |                     |                  |
                                  v                     v                  v
                                 FIFO                  LRU               Optimal
                                  |                     |                  |
                                  +---------------------+------------------+
                                                        |
                                                        v
                                               Simulation Reports
```

The page-replacement algorithms share a common report structure so their frame-by-frame traces and metrics can be displayed consistently.

---

## Project Structure

The following tree shows the main source layout and the implemented Virtual Memory module. Standalone test programs are maintained separately in the project root.

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
├── virtual_memory/
│   ├── Page.h
│   ├── Frame.h
│   ├── PageTableEntry.h
│   ├── PageTable.h
│   ├── PageTable.cpp
│   ├── VirtualMemoryManager.h
│   ├── VirtualMemoryManager.cpp
│   ├── PageReplacementTypes.h
│   ├── PageReferenceMapper.h
│   ├── FIFOReplacement.h
│   ├── FIFOReplacement.cpp
│   ├── FIFOPageReplacementSimulator.h
│   ├── FIFOPageReplacementSimulator.cpp
│   ├── LRUReplacement.h
│   ├── LRUReplacement.cpp
│   ├── LRUPageReplacementSimulator.h
│   ├── LRUPageReplacementSimulator.cpp
│   ├── OptimalReplacement.h
│   ├── OptimalReplacement.cpp
│   ├── OptimalPageReplacementSimulator.h
│   ├── OptimalPageReplacementSimulator.cpp
│   ├── VirtualMemoryUI.h
│   └── VirtualMemoryUI.cpp
│
├── output/
├── screenshots/
├── .gitignore
└── README.md
```

The exact set of test files may grow as additional modules and edge cases are implemented.

---

## Getting Started

### Prerequisites

- Windows 10 or Windows 11.
- GCC compiler with C++20 support.
- MinGW-w64.
- PowerShell.
- Visual Studio Code or another C++ IDE.

Check that GCC is available:

```powershell
g++ --version
```

### Clone the repository

```powershell
git clone https://github.com/subhadipp123/os-resource-management-simulator.git

cd os-resource-management-simulator
```

### Create the output directory

Run this from the project root if the directory does not already exist:

```powershell
mkdir output
```

---

## Build and Run

### Compile the complete application

Run this command from the project root directory:

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp menu.cpp `
process/Process.cpp `
process/ProcessManager.cpp `
process/ProcessManagementUI.cpp `
scheduling/CPUSchedulingUI.cpp `
scheduling/FCFSScheduler.cpp `
scheduling/GanttChart.cpp `
scheduling/PreemptivePriorityScheduler2.cpp `
scheduling/PriorityScheduler1.cpp `
scheduling/SchedulingValidator.cpp `
scheduling/SJFScheduler.cpp `
scheduling/SRTFScheduler.cpp `
performance/ManualInput.cpp `
performance/PerformanceAnalysisUI.cpp `
performance/SchedulingBenchmark.cpp `
performance/SchedulingReport.cpp `
performance/WorkloadGenerator.cpp `
memory/MemoryAllocationUI.cpp `
memory/MemoryManager.cpp `
virtual_memory/VirtualMemoryUI.cpp `
virtual_memory/FIFOPageReplacementSimulator.cpp `
virtual_memory/FIFOReplacement.cpp `
virtual_memory/LRUPageReplacementSimulator.cpp `
virtual_memory/LRUReplacement.cpp `
virtual_memory/OptimalPageReplacementSimulator.cpp `
virtual_memory/OptimalReplacement.cpp `
virtual_memory/VirtualMemoryManager.cpp `
virtual_memory/PageTable.cpp `
-o output/simulator.exe
```

`PageReferenceMapper.h` is a header-only utility and does not need a separate `.cpp` compilation input.

### Run the simulator

```powershell
.\output\simulator.exe
```

**Note:** Standalone test source files contain their own `main()` functions. Do not include those test files in the complete application build command.

---

## Main Menu

The current main menu contains five implemented modules and an exit option:

```text
========================================
       OS RESOURCE MANAGEMENT SIMULATOR
========================================
1. Process Management
2. CPU Scheduling
3. Performance Analysis
4. Memory Management
5. Virtual Memory & Page Replacement
6. Exit
```

### Virtual Memory submenu

```text
========================================
       VIRTUAL MEMORY MANAGEMENT
========================================
1. Run a Single Page Replacement Algorithm
2. Compare All Page Replacement Algorithms
3. Check Belady's Anomaly
4. Back to Main Menu
```

---

## Testing and Validation

The project uses standalone C++ test programs to verify individual components and edge cases. The complete application is also run through its menu to verify module integration.

### Process Management tests

- Process creation and automatic PID assignment.
- Process searching and termination.
- Valid and invalid process-state transitions.
- Process state management.

### CPU Scheduling tests

- Different process arrival and burst times.
- Simultaneous process arrivals.
- CPU idle intervals.
- Preemptive and non-preemptive scheduling.
- Priority-based scheduling.
- Scheduling metric calculations.
- Gantt chart generation.

### Performance Analysis tests

- Manual process input and validation.
- Comparison using identical process workloads.
- Automatic workload generation.
- Reproducible workload generation using random seeds.
- Benchmarking with different process counts.
- Repeated execution-time measurements.
- Median, minimum, and maximum execution-time calculations.
- Comparison report generation.
- Integrated menu navigation.

### Memory Management tests

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

### Virtual Memory tests

- Page and frame data structures.
- Page-table initialization.
- Valid and invalid page access.
- Page-to-frame mapping and unmapping.
- Virtual memory manager initialization.
- Rejection of zero and negative frame counts.
- Page hits and page faults.
- FIFO replacement behavior.
- LRU replacement behavior.
- Optimal replacement behavior.
- Single-frame and repeated-reference cases.
- More frames than unique referenced pages.
- Empty reference-string validation.
- Negative page-number validation.
- Page-reference scalability with large page numbers.
- Frame-by-frame trace consistency.
- Page-hit and page-fault totals.
- Belady's Anomaly detection.
- Integrated Virtual Memory menu execution.

**Recorded test results from the Virtual Memory development cycle**

| Test group | Result |
|---|---:|
| Page replacement edge-case checks (FIFO, LRU, Optimal) | 27 passed, 0 failed |
| Virtual Memory manager constructor checks | 3 passed, 0 failed |
| Large page-number scalability | Passed for FIFO, LRU, and Optimal |
| Belady's Anomaly example | FIFO: 9 faults with 3 frames, 10 faults with 4 frames |
| Algorithm comparison for the same anomaly example | FIFO: 9 → 10; LRU: 10 → 8; Optimal: 7 → 6 |

These are results from the test runs performed during development, not a claim that every possible input or environment has been exhaustively tested.

---

## Key Concepts Demonstrated

### Process management

- Process Control Block (PCB)-style process information.
- Process lifecycle.
- State transitions.
- Process identification and termination.

### CPU scheduling

- Arrival time and burst time.
- Preemption and non-preemption.
- Ready-queue selection.
- CPU idle time.
- Waiting, turnaround, response, and completion times.
- Gantt charts.
- Workload-based comparison.

### Memory management

- Contiguous memory allocation.
- First Fit, Best Fit, and Worst Fit.
- Splitting and coalescing.
- Free-space tracking.
- External fragmentation.

### Virtual memory

- Pages and frames.
- Page tables.
- Page hits and page faults.
- Demand-paging simulation.
- FIFO, LRU, and Optimal replacement.
- Hit ratio and page-fault rate.
- Belady's Anomaly.
- Compact mapping for sparse page-number values.

### Software engineering and C++

- Modular source organization.
- Header/source separation.
- Encapsulation and object-oriented design.
- STL containers and algorithms.
- Input validation.
- Standalone component testing.
- Compiler warnings with `-Wall -Wextra -pedantic`.
- Git-based version control.

---

## Current Scope and Limitations

This is an educational simulator, not a production operating system.

- Process records and scheduling are simulated; the application does not schedule actual OS processes.
- Memory allocation is modeled in data structures; the application does not allocate or control physical RAM.
- Virtual memory and page replacement are simulated; the application does not modify the host operating system's page tables or hardware memory-management unit.
- The Optimal page-replacement algorithm uses future references and is intended as a theoretical comparison baseline.
- The Belady's Anomaly checker compares the selected reference string at `N` and `N + 1` frames; it does not prove a universal property from one test.
- Benchmark execution times depend on the host computer, compiler, build configuration, and runtime conditions.

---

## Planned Enhancements

The following items are planned and should not be considered implemented until they are added and tested.

### 1. Deadlock Management

- Deadlock fundamentals and resource allocation.
- Resource Allocation Graph representation.
- Banker's Algorithm.
- Safe-state detection.
- Deadlock avoidance analysis.

### 2. Additional improvements

- More automated regression tests.
- Expanded documentation and algorithm notes.
- Screenshots and sample execution demonstrations.
- Additional performance analysis and visualization.
- Further algorithm optimization where measurement supports it.

---

## Learning Outcomes

This project provides practical experience with:

- Operating-system process management.
- Process lifecycle and state transitions.
- CPU scheduling algorithms.
- Preemptive and non-preemptive scheduling.
- Scheduling performance evaluation.
- Algorithm execution-time measurement.
- Contiguous memory allocation.
- Memory fragmentation and coalescing.
- Virtual memory and demand-paging concepts.
- Page tables, frames, and page replacement.
- FIFO, LRU, and Optimal algorithms.
- Belady's Anomaly.
- C++20 and the Standard Template Library.
- Modular software architecture.
- Input validation, testing, and debugging.
- Git and GitHub version control.

---

## Author

**Subhadip Pradhan**

- GitHub: https://github.com/subhadipp123
- Project repository: https://github.com/subhadipp123/os-resource-management-simulator

---

If you find an issue or want to suggest an improvement, feel free to open an issue or submit a pull request.
