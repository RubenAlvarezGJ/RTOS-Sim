#include "rtos.h"
#include "task.h"
#include "context.h"
#include <iostream>

/**
 * @brief Example usage of the RTOS kernel simulator.
 *
 * This file defines example task functions and shows how to create and start tasks using the RTOS.
 */

/**
 * @brief Task functions executed by the RTOS must follow a specific signature.
 * 
 * Each task function must:
 * - Have the signature 'void functionName(void* args)'.
 * - Accept a single 'void*' argument, which will point to a 'TaskContext' object.
 * - Use 'static_cast<TaskContext*>(args)' to retrieve task-specific information.
 * 
 * The 'TaskContext' struct provides:
 * - A pointer to the current 'Task' ('ctx->task')
 * - A pointer to the owning 'RTOS' instance ('ctx->rtos')
 * 
 * Task functions can read the current task's name, step, and other data,
 * and even create new tasks dynamically if needed.
 *
 * @see TaskContext
 * @see RTOS::createTask
 */
 
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
  if (step == 0) {
    ctx->rtos->createTask("DynamicallyCreatedTask", 3, exampleTask);
  }
}

int main() {
  bool standard = true;

  if (standard) {
    RTOS rtos1(5000); 

    rtos1.createTask("TaskZ", 5, exampleTask);
    rtos1.createTask("TaskA", 1, exampleTask);
    rtos1.createTask("TaskB", 1, exampleTask);
    rtos1.createTask("TaskC", 2, exampleTask);
    rtos1.createTask("TaskD", 3, exampleTask);
    rtos1.createTask("TaskE", 4, exampleTask);
    rtos1.createTask("TaskF", 1, exampleTask);

    rtos1.startScheduler();
  }

  else {
    // with dynamic scheduling
    RTOS rtos2(10000);

    rtos2.createTask("TaskA", 3, createDynamic);
    rtos2.createTask("TaskB", 3, exampleTask);
    rtos2.createTask("TaskC", 3, exampleTask);
    rtos2.createTask("TaskD", 2, exampleTask);
    rtos2.createTask("TaskE", 1, exampleTask);

    rtos2.startScheduler();
  }
}