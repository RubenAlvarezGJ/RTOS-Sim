#ifndef SCHEDUELR_H
#define SCHEDULER_H

#include "config.h"
#include "clock.h"
#include <vector>

class Task;

class Scheduler {
  public:
    Scheduler() = default;
    ~Scheduler();
    void addToReadyList(Task* task);
    void removeTask(Task* task);
    void run();
    
  private:
    Task* idleTask_ = nullptr;                               // idle tasks that executes when no other tasks available
    Clock clock_;                                            // clock to simulate ticks
    std::vector<Task*> readyLists_[configMAX_PRIORITY + 1];  // ready lists for each priortiy level

    void initializeIdleTask();
    void moveToBack(Task* toMove, uint8_t priorityIdx);
    Task* getHighestPriorityTask();
};

#endif