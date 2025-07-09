#ifndef SCHEDUELR_H
#define SCHEDULER_H

#include "config.h"
#include "clock.h"
#include <vector>

class Task;

class Scheduler {
  public:
    Scheduler() = default;
    void addToReadyList(Task* task);
    void removeTask(Task* task);
    void run();
    
  private:
    Task* idleTask_ = nullptr; // runs when no ready tasks are available
    Clock clock_;
    std::vector<Task*> readyLists_[configMAX_PRIORITY]; // ready lists for each priortiy level

    void initializeIdleTask();
    Task* getHighestPriorityTask();
};

#endif