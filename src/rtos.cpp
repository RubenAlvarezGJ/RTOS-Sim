#include "rtos.h"

void RTOS::createTask(std::string name, uint8_t priority, taskFunction_t taskCode, void* args) {
  if ( (nextFree_ + STACK_SIZE) > memorySize_) {
    std::cerr << "Insufficient memory available to create task\n";
  }

  uint8_t* stackBase = memoryPool_ + nextFree_; // gets the next available stack from the memory pool using nextFree_ as offset
  uint8_t* stackPtr = stackBase + STACK_SIZE; // sets stackPtr to the top of the newly allocated stack (stack grows down)

  Task* task = new Task(name, priority, stackBase, stackPtr, taskCode, args);
  taskList_.push_back(task); // for debugging purposes

  scheduler_.addToReadyList(task);

  updateNextFree();
}

void RTOS::startScheduler() {
  return; // function stub
}