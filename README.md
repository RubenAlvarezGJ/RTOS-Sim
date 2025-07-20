# RTOS-Simulator

This is a RTOS (Real-Time Operating System) kernel simulator written in C++ loosely inspired by the FreeRTOS Kernel. I wanted to learn how real-time operating systems differed from traditional operating systems so I decided to write a lightweight kernel simulator that mainly focuses on task-creation, scheduling, and memory management within a simulated environment. 

## Key Features
- **Custom Task Objects**: Task objects represent a simulated task in the rtos and are associated with a priority, state, step counter, and code to execute.

- **Preempitive Priority-Based Scheduling**: Tasks are managed in ready queues organized by priority level. Higher priority tasks preempt lower priority ones.

- **Round-Robin Scheduling**: For tasks with equal priority, simulated CPU time is divided equally in round-robin fashion.

- **Memory Pool**: Used to simulate memory management and task specific stack allocation.

- **Task Lifecycle Management**: Tasks progress through simulated steps. When a task completes all steps, it is removed from its ready queue marking its completion.

## Architecture Overview

### `RTOS`  
This is the main API interface. Used by the user to create tasks and start the scheduler.

### `Scheduler`  
Handles task scheduling and execution:
- Organizes tasks into multiple priority queues
- Uses fixed-priority preemptive scheduling
- Implements round-robin for equal-priority tasks

### `Task`  
Includes:
- Task name and priority
- Step count for simulation
- Status (BLOCKED, READY, RUNNING, SUSPENDED)
- A function pointer to the task logic

## How It Works

1. **Task Creation**  
    ```cpp
    rtos.createTask("TaskA", 3, exampleTask); // This creates a task of priority 3 and adds it to the ready queue.

2. **Scheduler Invocation**
    ```cpp
    rtos.startScheduler(); // This starts the scheduler and begins executing tasks based on priority level.

3. **Task Execution**  
Tasks are executed for a number of steps. Each tick may result in task switching, either due to preemption or round-robin rotation.

4. **Task Function Structure**  
All task functions follow this format for the purpose of simulation:
    ```cpp
    // Example of a basic task function
    void exampleTask(void* args) {
        TaskContext* ctx = static_cast<TaskContext*>(args);
        std::size_t step = ctx->task->getStep();
        std::cout << "'" << ctx->task->getName() << "' executing step " << static_cast<int>(step) << "\n";
    }

    // Example of a task that creates another task dynamically
    void createDynamic(void* args) {
        TaskContext *ctx = static_cast<TaskContext*>(args);
        std::size_t step = ctx->task->getStep();
        std::cout << "'" << ctx->task->getName() << "' executing step " << static_cast<int>(step) << "\n";
        if (step == 0) { // or any other step
            ctx->rtos->createTask("DynamicallyCreatedTask", 3, exampleTask);
        }
    }

## Testing
I used the testing framework [Catch2](https://github.com/catchorg/Catch2). To run the test suite, build the project and run the **tests** executable.


## Build Instructions

### Prerequisites

- CMake 3.11 or higher
- A C++17-compatible compiler (e.g. clang++, g++, MSVC(for windows))
- Git

### Build Steps

- Clone the repo with:
```
git clone https://github.com/RubenAlvarezGJ/RTOS-Sim.git
```
- Nagivate to the project directory with:
```
cd RTOS-Sim
```
- Then run the following script:

### Linux/macOS

```bash
./build.sh
```

### Windows
```bash
./build.ps1
```