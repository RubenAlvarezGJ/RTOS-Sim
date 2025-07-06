#ifndef RTOS_H
#define RTOS_H

#include "config.h"
#include "common.h"
#include <vector>
#include <iostream>

class Task;
class Scheduler;

class RTOS {
  public:
    RTOS(size_t memorySize) : memorySize_(memorySize), nextFree_(0) {
      if (memorySize_ > configMAX_RAM_SIZE) {
        std::cerr << "Requested memory size exceeds maximum RAM available\n";
      }
      memoryPool_ = new uint8_t[memorySize_];
    }
    void createTask(std::string name, uint8_t priority, taskFunction_t taskCode, void* args); // creates a new task and adds it to the scheduler
    void startScheduler();

  private:
    void updateNextFree() {nextFree_ += configSTACK_SIZE;}
  
    Scheduler* scheduler_;
    uint8_t* memoryPool_; // simulated system RAM
    size_t memorySize_; // size of memoryPool
    size_t nextFree_; // pointer to offset into the memory pool
    std::vector<Task*> taskList_;
};

#endif