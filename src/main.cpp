#include "rtos.h"
#include "task.h"
#include <iostream>


/*
  Example task function that will be executed by the RTOS.
  This is what all functions that are passed to tasks should look like for this simulator
*/
void exampleTask(void* args) {
  Task* task = static_cast<Task*>(args);
  std::size_t* step = task->getStep();
  std::cout << "'" << task->getName() << "' executing step " << static_cast<int>(*step) << "\n";
  (*step)++;
}

int main() {
  RTOS rtos(1000); // initialize the simulator with 1000 "bytes" of RAM

  rtos.createTask("TaskA", 1, exampleTask);
  rtos.createTask("TaskB", 1, exampleTask);
  rtos.createTask("TaskC", 2, exampleTask);
  rtos.createTask("TaskD", 3, exampleTask);
  rtos.createTask("TaskE", 4, exampleTask);

  rtos.startScheduler();

  return 0;
}