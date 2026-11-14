# OS Resource Management Simulator

A modular, C++20-based Operating System Resource Management Simulator that demonstrates how operating systems manage processes, CPU scheduling, memory allocation, virtual memory, deadlocks, and synchronization.

The project provides an interactive environment for experimenting with operating system algorithms, evaluating their performance, and understanding resource-management strategies through simulation.

## Overview

Operating systems coordinate multiple processes while managing limited resources such as CPU time, physical memory, and shared resources.

This simulator models these core responsibilities through independent modules, allowing users to execute different algorithms, compare their behaviour, and analyze performance metrics.

## Key Features

- **Process Management:** Create, search, manage, and terminate processes with unique process IDs and validated state transitions.
- **CPU Scheduling:** Execute and compare multiple CPU scheduling algorithms.
- **Memory Management:** Simulate memory allocation strategies and analyze fragmentation.
- **Virtual Memory:** Implement page replacement algorithms and track page faults and hits.
- **Deadlock Management:** Analyze safe states and resource allocation using Banker's Algorithm.
- **Concurrency and Synchronization:** Demonstrate thread synchronization and shared-resource management.
- **Performance Analysis:** Calculate algorithmic performance metrics and compare execution results.
- **Data Export:** Export simulation results in CSV format for further analysis.

## Modules and Algorithms

### 1. Process Management

Simulates the process lifecycle and maintains process information.

- Process creation and termination
- Automatic PID assignment
- Process states:
  - New
  - Ready
  - Running
  - Waiting
  - Terminated
- Process searching and state-transition validation
- Process information management

### 2. CPU Scheduling

Implements and compares CPU scheduling algorithms:

| Algorithm | Description |
|---|---|
| FCFS | First Come First Serve |
| SJF | Shortest Job First |
| SRTF | Shortest Remaining Time First |
| Priority Scheduling | Executes processes according to priority |
| Round Robin | Allocates CPU time using a time quantum |

**Performance Metrics:**
- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- Completion Time
- CPU Utilization
- Throughput

### 3. Memory Management

Simulates memory allocation using:

- First Fit
- Best Fit
- Worst Fit

**Performance Analysis:**
- Memory utilization
- Internal fragmentation
- External fragmentation
- Allocation success and failure

### 4. Virtual Memory and Page Replacement

Simulates virtual memory using page-reference sequences and configurable frame counts.

**Algorithms:**
- FIFO (First In First Out)
- LRU (Least Recently Used)
- Optimal Page Replacement

**Performance Metrics:**
- Total Page Faults
- Total Page Hits
- Page Fault Ratio
- Page Hit Ratio

### 5. Deadlock Management

Demonstrates deadlock avoidance and resource allocation concepts.

- Resource Allocation
- Available, Allocated, and Maximum resource tracking
- Banker's Algorithm
- Safe and Unsafe State Detection
- Safe Sequence Generation
- Resource Request Validation

### 6. Concurrency and Synchronization

Demonstrates synchronization challenges in concurrent execution.

- Multithreading using C++
- Mutex-based synchronization
- Semaphore-based synchronization
- Critical Section Management
- Race Condition Demonstrations
- Producer-Consumer Problem

## Technology Stack

| Technology | Purpose |
|---|---|
| C++20 | Core programming language |
| STL | Data structures and algorithms |
| OOP | Modular and maintainable architecture |
| C++ Threads | Concurrent execution |
| Mutexes and Semaphores | Synchronization |
| Git and GitHub | Version control |
| CSV | Simulation result export |

## Project Architecture

The application follows a modular architecture with separate components for process management, scheduling, memory management, virtual memory, deadlock handling, and synchronization.

```text
OS-Resource-Management-Simulator/
│
├── src/
│   ├── main.cpp
│   ├── menu/
│   ├── process/
│   ├── scheduling/
│   ├── memory/
│   ├── virtual_memory/
│   ├── deadlock/
│   ├── synchronization/
│   └── benchmarking/
│
├── include/
├── tests/
├── benchmarks/
├── output/
├── screenshots/
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

*Directory structure shown as the intended final organization.*

## Performance Analysis

The simulator evaluates algorithm behaviour using consistent input workloads.

Examples include:

- Comparing CPU scheduling algorithms using identical process workloads.
- Evaluating page replacement algorithms using identical reference strings and frame counts.
- Comparing memory allocation strategies using identical memory configurations.
- Measuring execution time for selected simulation workloads.

Results can be exported as CSV files for additional analysis and visualization.

## Testing

The project includes tests for individual modules and important edge cases.

Testing covers:

- Process creation and PID uniqueness
- Invalid process inputs
- Valid and invalid process-state transitions
- CPU scheduling calculations
- Memory allocation and fragmentation
- Page replacement behaviour
- Deadlock safety checks
- Synchronization correctness
- Boundary conditions and invalid inputs

## Getting Started

### Prerequisites

- C++20-compatible compiler (GCC 11+ recommended)
- CMake 3.20+
- Visual Studio Code or another C++ IDE
- Git

### Clone the Repository

```bash
git clone https://github.com/subhadipp123/os-resource-management-simulator.git

cd os-resource-management-simulator
```

### Build the Project

```bash
cmake -S . -B build
cmake --build build
```

### Run the Simulator

Run the generated executable from the build directory. The executable name and exact path depend on the configured build system and operating system.

## Learning Objectives

This project demonstrates practical applications of:

- Operating System fundamentals
- Process lifecycle and process control
- CPU scheduling algorithms
- Memory allocation techniques
- Virtual memory and page replacement
- Deadlock avoidance
- Multithreading and synchronization
- Object-Oriented Programming
- Data structures and algorithm analysis
- Software modularity and testing

## Future Enhancements

Potential extensions include:

- Interactive Gantt chart visualization
- Graphical performance dashboards
- More scheduling and memory-management algorithms
- Large-scale configurable workloads
- Advanced benchmark reports
- Additional operating system simulation scenarios

## Author

**Subhadip Pradhan**

B.Tech Computer Science and Engineering

GitHub: [subhadipp123](https://github.com/subhadipp123)

---

**Note:** This is an educational operating system simulator. It models operating system concepts and algorithms; it does not modify or replace the host operating system's kernel or actual process scheduler.