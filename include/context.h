#ifndef TASK_CONTEXT_H
#define TASK_CONTEXT_H

#include <cstddef>

class Task;
class RTOS;

/**
 * @brief Represents the execution context passed to task functions in the RTOS simulation.
 *
 * TaskContext encapsulates the minimal information a task function needs to execute within
 * the simulated RTOS environment. It provides access to:
 * 
 * - A pointer to the Task object representing the currently executing task.
 * - A pointer to the RTOS instance, enabling the task to invoke RTOS APIs (e.g., dynamic task creation).
 */
struct TaskContext {
    Task* task = nullptr;
    RTOS* rtos = nullptr;
};

#endif