# RTOS-Simulator

A lightweight Real-Time Operating System (RTOS) kernel simulator written in C++, loosely inspired by the FreeRTOS Kernel.
This project simulates core RTOS concepts like task creation, preemptive priority-based scheduling, round-robin execution, and dynamic task management.

## Key Features
- **Custom Task Objects**: Each task has its own name, priority, state (READY, RUNNING, BLOCKED, SUSPENDED), step counter, and logic function.
- **Preemptive Priority-Based Scheduling**: Higher-priority tasks "preempt" lower ones.
- **Round-Robin Scheduling**: Equal-priority tasks share simulated CPU time in a rotating fashion.
- **Simulated Memory Pool**: Each task gets a simulated stack allocated from a memory pool.
- **Dynamic Task Creation**: Tasks can create new tasks at runtime from within their own logic.

---

## Architecture Overview

### `RTOS`  
Main API interface. Provides methods to create tasks and start the scheduler.

### `Scheduler`
Manages ready lists and selects which task to run. Implements:
- Fixed-priority preemptive scheduling
- Round-robin for equal-priority tasks
- Idle task fallback

### `Task`
A simulated thread-like object. Contains:
- Task name and priority
- Step count to simulate task progression
- State
- Function pointer to simulate user-defined task logic

---

## How It Works

1. **Task Creation**  
    ```cpp
    rtos.createTask("TaskA", 3, exampleTask); // This creates a task of priority 3 and adds it to the ready queue.
    ```

2. **Scheduler Invocation**
    ```cpp
    rtos.startScheduler(); // Begins task execution. The highest-priority READY task is selected at every tick.
    ```

3. **Task Execution**  
Tasks are executed for a number of steps. Each tick may result in task switching, either due to preemption or round-robin rotation.
When a task completes all of its steps, it is removed from the ready queue, marking its completion.

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
    ```

---

## Testing
I used the testing framework [**Catch2**](https://github.com/catchorg/Catch2). To run the test suite, build the project and run the **tests** executable.

---

## Build Instructions

### Prerequisites

- CMake 3.11 or higher
- A C++17-compatible compiler (e.g. clang++, g++, MSVC (for Windows))
- Git

### Build Steps

- Clone the repo with:
```
git clone https://github.com/RubenAlvarezGJ/RTOS-Sim.git
```
- Navigate to the project directory with:
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
