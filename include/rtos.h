#ifndef RTOS_H
#define RTOS_H

#include "task.h"
#include <vector>
#include <iostream>

constexpr std::size_t STACK_SIZE = 256; // 256 byte stack by default, change at your own risk
constexpr std::size_t MAX_RAM_SIZE = 16382; // maximum allowed RAM size (16 KB) change at your own risk

class RTOS {
  public:
    RTOS(size_t memorySize) : memorySize_(memorySize), nextFree_(0) {
      if (memorySize_ > MAX_RAM_SIZE) {
        std::cerr << "Requested memory size exceeds maximum RAM available\n";
      }
      memoryPool_ = new uint8_t[memorySize_];
    }
    bool createTask(std::string name, uint8_t priority, taskFunction_t taskCode, void *args); // creates a new task and adds it to the scheduler (scheduler not yet implemented)

  private:
    void updateNextFree() {nextFree_ += STACK_SIZE;}
  
    uint8_t *memoryPool_; // simulated system RAM
    size_t memorySize_; // size of memoryPool
    size_t nextFree_; // pointer to offset into the memory pool
    std::vector<Task*> taskList_;
    
};

#endif