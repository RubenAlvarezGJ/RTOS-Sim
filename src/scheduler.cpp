#include "scheduler.h"
#include "task.h"
#include <iostream>

void Scheduler::addToReadyList(Task* task) {
  uint8_t priority = task->getPriority(); // will be used as an index
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
      }}, nullptr);

      readyLists_[0].push_back(idleTask_);
    }
}

// TODO: most chosen task to the end to allow for round-robin scheduling
Task* Scheduler::getHighestPriorityTask() {
  int priority = static_cast<int>(configMAX_PRIORITY) - 1;

  for (priority; priority >= 0; --priority) {
    if (!readyLists_[priority].empty()) {
      return readyLists_[priority].front();
    }
  }

  return idleTask_; // idleTask_ is returned if no ready tasks are available.
}

void Scheduler::run() {
  initializeIdleTask();

  while (clock_.getTickCount() < configMAX_TICKS) {
    Task* next = this->getHighestPriorityTask();
    if (next->getName() != "Idle") {
      next->executeTask();
    }
    else {
      idleTask_->executeTask();
    }
    clock_.tick();
  }
}