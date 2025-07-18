#include "rtos.h"
#include "task.h"
#include "context.h"
#include <iostream>

// Example task functions that will be executed by the RTOS.
// This is what all functions that are passed to tasks should look like for this simulator.

void exampleTask(void* args) {
  TaskContext* ctx = static_cast<TaskContext*>(args);
  std::size_t step = ctx->task->getStep();
  std::cout << "'" << ctx->task->getName() << "' executing step " << static_cast<int>(step) << "\n";
}

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