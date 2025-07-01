#ifndef SCHEDUELR_H
#define SCHEDULER_H

#include "task.h"
#include <vector>
#include <queue>

class Scheduler {
  public:
    void addToReadyList(Task* task);
    void run();
  

  private:
    std::priority_queue<Task*, std::vector<Task*>, TaskComparator> taskList_; // stores pointers to Task objects. Task with highest priority stored at front of the queue.

};

#endif