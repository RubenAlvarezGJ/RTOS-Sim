#include "rtos.h"
#include "task.h"
#include "scheduler.h"
#include "context.h"
#include "output_control.h"

RTOS::RTOS(size_t memorySize) : memorySize_(memorySize), nextFree_(0) {
  if (memorySize_ > configMAX_RAM_SIZE) {
    if (!suppressOutput) {
      std::cerr << "Requested memory size exceeds maximum, using max allowed instead: " << configMAX_RAM_SIZE << "\n";
    }
    memorySize_ = configMAX_RAM_SIZE;
  }
  memoryPool_ = new uint8_t[memorySize_];
  scheduler_ = new Scheduler();
}

RTOS::~RTOS() {
  delete[] memoryPool_;
  delete scheduler_;
  for (auto task : taskList_) {
    TaskContext* ctx = static_cast<TaskContext*>(task->getArgs());
    if (ctx) delete ctx;
    delete task; 
  }
}

void RTOS::createTask(const std::string& name, uint8_t priority, taskFunction_t taskCode) {
  if ( (nextFree_ + configSTACK_SIZE) > memorySize_) {
    if (!suppressOutput) {
      std::cerr << "TASK CREATION FAILED: Insufficient memory available to create '" << name << "'\n";
    }
    return;
  }

  if ( (priority < 0) || (priority > configMAX_PRIORITY) ) {
    if (!suppressOutput) {
      std::cerr << "TASK CREATION FAILED: Priority level of " << name << " must be between 0 and " << configMAX_PRIORITY << " (set to " << static_cast<int>(priority) << ") \n";
    }
    return;
  }

  uint8_t* stackBase = memoryPool_ + nextFree_;      // gets the next available stack from the memory pool using nextFree_ as offset
  uint8_t* stackPtr = stackBase + configSTACK_SIZE;  // sets stackPtr to the top of the newly allocated stack (stack grows down)
  
  TaskContext* ctx = new TaskContext();              // context needed for taskCode execution
  Task* task = new Task(name, priority, stackBase, stackPtr, taskCode, ctx);
  ctx->task = task;
  ctx->rtos = this;

  taskList_.push_back(task);                        // currently for de-allocation purposes (might change)

  scheduler_->addToReadyList(task);

  updateNextFree();
}

void RTOS::startScheduler() {
  if (scheduler_->isRunning()) {
    if (!suppressOutput) {
      std::cerr << "SCHEDULER ERROR: Scheduler is already running.\n";
    }
    return;
  }
  scheduler_->run();
}