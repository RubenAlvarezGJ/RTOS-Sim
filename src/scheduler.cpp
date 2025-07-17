#include "scheduler.h"
#include "task.h"
#include <iostream>
#include <algorithm>

void Scheduler::initializeIdleTask() {
  if (idleTask_ == nullptr) {
    static uint8_t idleStack[128];
    uint8_t* stackBase = idleStack;
    uint8_t* stackPtr = stackBase + sizeof(idleStack);

    idleTask_ = new Task("Idle", 0, stackBase, stackPtr, [] (void*) {
      std::cout << "Idle task running...\n";
      }, nullptr);
      
    readyLists_[0].push_back(idleTask_);
  }
}

void Scheduler::moveToBack(Task* toMove, uint8_t priorityIdx) {
  if (!readyLists_[priorityIdx].empty()) {
    readyLists_[priorityIdx].erase(readyLists_[priorityIdx].begin()); 
    readyLists_[priorityIdx].push_back(toMove);
  }
}

Task* Scheduler::getHighestPriorityTask() {
  int priority = static_cast<int>(configMAX_PRIORITY);

  for (priority; priority >= 0; --priority) {
    if (!readyLists_[priority].empty()) {
      Task* task = readyLists_[priority].front();  // return the first task in the highest priority ready list
      task->setState(Task::RUNNING);
      return task;
    }
  }
  return idleTask_;                                // idleTask_ is returned if no ready tasks are available
}

void Scheduler::addToReadyList(Task* task) {
  uint8_t priority = task->getPriority();
  readyLists_[priority].push_back(task);
}

void Scheduler::removeTask(Task* task) {
  uint8_t priority = task->getPriority();
  auto& readyList = readyLists_[priority];

  auto rlTask = std::find(readyList.begin(), readyList.end(), task);
  if (rlTask != readyList.end()) {
    readyList.erase(rlTask);
  } 
  else {
    std::cerr << "Task Removal Failed: Task '" << task->getName() << "' not found in ready list.\n";
    return;
  }
}

void Scheduler::run() {
  initializeIdleTask();

  while (clock_.getTickCount() < configMAX_TICKS) {
    Task* next = this->getHighestPriorityTask();
    
    if (next->getName() != "Idle") {
      next->executeTask();
      next->incrementStep();
      if ((next->getStep()) >= configNUM_TASK_STEPS) {
        removeTask(next);
      }
      else {
        moveToBack(next, next->getPriority()); // move current task to the back of it's priority list to allow for round-robin scheduling
      }
    }
    else {
      idleTask_->executeTask();
    }
    clock_.tick();
  }
  std::cout << "MAXIMUM NUMBER OF CLOCK TICKS REACHED.\n";
}