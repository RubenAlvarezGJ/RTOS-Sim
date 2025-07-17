#include "task.h"
#include "config.h"
#include "output_control.h"
#include <iostream>

uint8_t Task::getPriority() const {
  return this->priority_;
}

uint8_t Task::getState() const {
  return this->state_;
}

std::size_t Task::getStep() {
  return this->taskStep_;
}

std::string Task::getName() const {
  return this->name_;
}

void Task::setPriority(uint8_t priority) {
  if (priority > configMAX_PRIORITY) {
    if (!suppressOutput) {
      std::cerr << "Chosen priority exceeds maximum allowed priority of " << configMAX_PRIORITY << "\n";
    }
    return;
  }
  this->priority_ = priority;
}

void Task::setState(uint8_t state) {
  if (state > 3) {
    if (!suppressOutput) {
      std::cerr << "State must be BLOCKED (0), RUNNING (1), READY (2) or SUSPENDED (3)\n";
    }
    return;
  }
  this->state_ = state;
}

void Task::incrementStep() {
  taskStep_++;
}

void* Task::getArgs() {
  if (!args_ && !suppressOutput) {
    std::cerr << "Invalid task arguments pointer\n";
  }
  return args_;
}

void Task::executeTask() {
  if (!funcPtr_) {
    if (!suppressOutput)  {
      std::cerr << "Invalid function pointer\n";
    }
    return;
  }
  funcPtr_(args_); 
}