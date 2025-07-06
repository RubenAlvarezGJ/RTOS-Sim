#ifndef TASK_H
#define TASK_H

#include "common.h"
#include <cstdint>
#include <string>

#define BLOCKED   0
#define RUNNING   1
#define READY     2
#define SUSPENDED 3

class Task {
  public:
    Task(std::string name, uint8_t priority, uint8_t* stackBase, uint8_t* stackPtr, taskFunction_t taskCode, void* args)
        : name_(name), priority_(priority), stackBase_(stackBase), stackPtr_(stackPtr), funcPtr_(taskCode), funcArgs_(args),
          state_(READY) {}
    void setPriority(uint8_t priority);
    void setState(uint8_t state); 
    uint8_t getPriority() const;
    uint8_t getState() const;
    std::string getName() const;
    void executeTask(); 

  private:
    std::string name_; // for debugging purposes
    uint8_t priority_; // ranges from 0 to configMAX_PRIORITY (see config.h)
    uint8_t state_; // blocked, running, ready or suspended
    uint8_t* stackBase_; // base of the stack 
    uint8_t* stackPtr_; // simulated stack pointer 
    std::size_t stackSize_ = 256; // size of stack allocated for this task (fixed 256 bytes)
    taskFunction_t funcPtr_; // code that the task executes when given cpu time
    void* funcArgs_; // arguments to be passed into the task function
};

#endif // TASK_H