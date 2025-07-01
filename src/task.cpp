#include "task.h"

void Task::setPriority(uint8_t priority) {
  this->priority_ = priority;
}

void Task::setState(uint8_t state) {
  this->state_ = state;
}

uint8_t Task::getPriority() const {
  return this->priority_;
}
uint8_t Task::getState() const {
  return this->state_;
}

void Task::executeTask() {
  if (funcPtr_ != nullptr) {
    funcPtr_(funcArgs_);
  }
}