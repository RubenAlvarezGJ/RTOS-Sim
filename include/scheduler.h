#ifndef SCHEDUELR_H
#define SCHEDULER_H

#include "config.h"
#include "clock.h"
#include <vector>

class Task;

/**
 * @class Scheduler
 * @brief Simulates a priority-based task scheduler for the RTOS.
 * 
 * Handles task selection, round-robin scheduling within priorities, and fallback to an idle task.
 */
class Scheduler {
  public:
    /**
     * @brief Default constructor for Scheduler.
     */
    Scheduler() = default;

    /**
     * @brief Destructor for Scheduler.
     * 
     * Frees memory associated with the idle task.
     */
    ~Scheduler();

    /**
     * @brief Adds a task to the appropriate ready list based on its priority.
     * 
     * @param task The task to add.
     */
    void addToReadyList(Task* task);

    /**
     * @brief Removes a task from its ready list.
     * 
     * If the task is not found, a warning is printed (if output is not suppressed e.g. in testing).
     * 
     * @param task The task to remove.
     */
    void removeTask(Task* task);

    /**
     * @brief Returns whether the scheduler is currently running.
     * 
     * @return true if running, false otherwise.
     */
    bool isRunning();

    /**
     * @brief Starts the task scheduler and simulates task execution.
     * 
     * Runs until the simulated tick count reaches configMAX_TICKS.
     */
    void run();
    
  private:
    void initializeIdleTask();
    void moveToBack(Task* toMove, uint8_t priorityIdx);
    Task* getHighestPriorityTask();

  private:
    bool isRunning_ = false;
    Task* idleTask_ = nullptr;                               // idle tasks that executes when no other tasks available
    Clock clock_;                                            // clock to simulate ticks
    std::vector<Task*> readyLists_[configMAX_PRIORITY + 1];  // ready lists of Tasks for each priortiy level
};

#endif