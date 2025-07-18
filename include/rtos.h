#ifndef RTOS_H
#define RTOS_H

#include "config.h"
#include "common.h"
#include <vector>
#include <iostream>

class Task;
class Scheduler;

/**
 * @class RTOS
 * @brief Simulated Real-Time Operating System (RTOS) kernel.
 *
 * The RTOS manages task creation, memory allocation for task stacks, and scheduling.
 */
class RTOS {
  public:
    /**
     * @brief Construct a new RTOS object with a given memory size.
     * 
     * If the specified memory exceeds configMAX_RAM_SIZE, it is clamped.
     * 
     * @param memorySize Total simulated memory available for stack allocation.
     */
    RTOS(size_t memorySize);

    /**
     * @brief Destructor for the RTOS.
     * 
     * Frees allocated memory and destroys the scheduler and all tasks.
     */
    ~RTOS();

    /**
     * @brief Creates a new task and adds it to the scheduler.
     * 
     * @param name Name of the task.
     * @param priority Task priority (0 = lowest, configMAX_PRIORITY = highest).
     * @param taskCode Pointer to the function the task should execute.
     */
    void createTask(const std::string& name, uint8_t priority, taskFunction_t taskCode);

    /**
     * @brief Starts the RTOS scheduler.
     * 
     * If already running, does nothing.
     */
    void startScheduler();

  private:
    void updateNextFree() {nextFree_ += configSTACK_SIZE;}
  
    Scheduler* scheduler_;
    uint8_t* memoryPool_;          // simulated system RAM
    size_t memorySize_;            // size of memoryPool
    size_t nextFree_;              // pointer to offset into the memory pool
    std::vector<Task*> taskList_;  // currently being used for de-allocation
};

#endif