#ifndef TASK_H
#define TASK_H

#include "common.h"
#include <cstdint>
#include <string>

class Task {
  public:
    Task(const std::string& name, uint8_t priority, uint8_t* stackBase, uint8_t* stackPtr, taskFunction_t taskCode)
        : name_(name), priority_(priority), stackBase_(stackBase), stackPtr_(stackPtr), funcPtr_(taskCode),
          state_(READY), taskStep_(0) {}
    void setPriority(uint8_t priority);
    void setState(uint8_t state); 
    uint8_t getPriority() const;
    uint8_t getState() const;
    std::size_t* getStep();
    std::string getName() const;
    void executeTask(); 

    static constexpr uint8_t BLOCKED   = 0;
    static constexpr uint8_t RUNNING   = 1;
    static constexpr uint8_t READY     = 2;
    static constexpr uint8_t SUSPENDED = 3;

  private:
    uint8_t priority_; // ranges from 0 to configMAX_PRIORITY (see config.h)
    uint8_t state_; // blocked, running, ready or suspended
    uint8_t* stackBase_; // base of the stack 
    uint8_t* stackPtr_; // simulated stack pointer 
    std::string name_; // for debugging purposes
    std::size_t taskStep_; // step counter for this task
    std::size_t stackSize_ = 256; // size of stack allocated for this task (fixed 256 bytes)
    taskFunction_t funcPtr_; // code that the task executes when given cpu time
};

#endif // TASK_H