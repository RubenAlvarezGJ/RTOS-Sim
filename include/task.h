#ifndef TASK_H
#define TASK_H

#include <cstdint>

#define BLOCKED   0
#define RUNNING   1
#define READY     2
#define SUSPENDED 3

class Task {
  public:
    Task(uint8_t priority = 0, uint8_t state = READY, void *funcArgs, uint32_t size, void (*func)(void *args));
    void setPriority(uint8_t priority);
    void setState(uint8_t state); // NOTE: might not need 
    void executeTask();

  private:
    uint8_t priority; // ranges from 0 to MAX_PRIORITY defined elsewhere
    uint8_t state; // blocked, running, ready or suspended
    void *funcArgs; // arguments to be passed into the task function
    char *stackPtr; // points to the top of the stack
    uint32_t stackSize; // size of stack allocated for this task
    void (*funcPtr)(void *args); // code that the task executes when given cpu time
};


#endif // TASK_H