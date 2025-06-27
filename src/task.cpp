#include "task.h"

void Task::setPriority(uint8_t priority) {
  this->priority_ = priority;
}

void Task::setState(uint8_t state) {
  this->state_ = state;
}

void Task::executeTask() {
  if (funcPtr_ != nullptr) {
    funcPtr_(funcArgs_);
  }
}