#include "task.h"

Task::Task(uint8_t priority = 0, uint8_t state = READY, void *funcArgs, uint32_t size, void (*func)(void *args)) {
  this->priority = priority;
  this->state = state;
  this->funcArgs = funcArgs; // might not be quite right
  this->stackSize = size;
  funcPtr = func;
  // need to figure out how to initialize stackPtr properly
}

void Task::setPriority(uint8_t priority) {
  this->priority = priority;
}

void Task::setState(uint8_t state) {
  this->state = state;
}

void Task::executeTask() {
  if (funcPtr != nullptr) {
    funcPtr(funcArgs);
  }
}