#include "task.h"
#include "config.h"
#include <iostream>

void Task::setPriority(uint8_t priority) {
  if (priority > configMAX_PRIORITY) {
    std::cerr << "Chosen priority exceeds maximum allowed priority of " << configMAX_PRIORITY << "\n";
  }
  this->priority_ = priority;
}

void Task::setState(uint8_t state) {
  if (state > 3) {
    std::cerr << "State must be BLOCKED (0), RUNNING (1), READY (2) or SUSPENDED (3)\n";
  }
  this->state_ = state;
}

uint8_t Task::getPriority() const {
  return this->priority_;
}

uint8_t Task::getState() const {
  return this->state_;
}
std::string Task::getName() const {
  return this->name_;
}

std::size_t* Task::getStep() {
  return &taskStep_;
}

void Task::executeTask() {
  if (funcPtr_) funcPtr_(this); 
}