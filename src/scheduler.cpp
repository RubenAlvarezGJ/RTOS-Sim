#include "scheduler.h"
#include "task.h"
#include <iostream>
#include <algorithm>

void Scheduler::addToReadyList(Task* task) {
  uint8_t priority = task->getPriority();
  readyLists_[priority].push_back(task);
}

void Scheduler::initializeIdleTask() {
  if (idleTask_ == nullptr) {
    static uint8_t idleStack[128];
    uint8_t* stackBase = idleStack;
    uint8_t* stackPtr = stackBase + sizeof(idleStack);

    idleTask_ = new Task("Idle", 0, stackBase, stackPtr, [] (void*) {
      while (true) {
        std::cout << "Idle task running...\n";
        break;  // for simulation purposes: don't actually loop forever (will need to change later to allow for interrupts)
      }});

      readyLists_[0].push_back(idleTask_);
    }
}

Task* Scheduler::getHighestPriorityTask() {
  int priority = static_cast<int>(configMAX_PRIORITY) - 1;

  for (priority; priority >= 0; --priority) {
    if (!readyLists_[priority].empty()) {
      // return the first task in the highest priority ready list, then move it to the end of the list to allow for round-robin scheduling
      Task* task = readyLists_[priority].front();
      readyLists_[priority].erase(readyLists_[priority].begin()); 
      readyLists_[priority].push_back(task); 
      task->setState(Task::RUNNING); 
      return task; 
    }
  }

  return idleTask_; // idleTask_ is returned if no ready tasks are available.
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
  }
}

void Scheduler::run() {
  initializeIdleTask();

  while (clock_.getTickCount() < configMAX_TICKS) {
    Task* next = this->getHighestPriorityTask();
    if (next->getName() != "Idle") {
      next->executeTask();
      if (*(next->getStep()) >= configNUM_TASK_STEPS) {
        removeTask(next);
      }
    }
    else {
      idleTask_->executeTask();
    }
    clock_.tick();
  }
}