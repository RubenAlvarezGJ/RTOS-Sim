#include "rtos.h"
#include "task.h"
#include "scheduler.h"

RTOS::RTOS(size_t memorySize) : memorySize_(memorySize), nextFree_(0) {
  if (memorySize_ > configMAX_RAM_SIZE) {
    std::cerr << "Requested memory size exceeds maximum RAM available\n";
  }
  memoryPool_ = new uint8_t[memorySize_];
  scheduler_ = new Scheduler();
}

RTOS::~RTOS() {
  delete[] memoryPool_;
  delete scheduler_;
  for (auto task : taskList_) {
    delete task; 
  }
}

void RTOS::createTask(const std::string& name, uint8_t priority, taskFunction_t taskCode) {
  if ( (nextFree_ + configSTACK_SIZE) > memorySize_) {
    std::cerr << "Insufficient memory available to create '" << name << "'\n";
    return;
  }

  uint8_t* stackBase = memoryPool_ + nextFree_; // gets the next available stack from the memory pool using nextFree_ as offset
  uint8_t* stackPtr = stackBase + configSTACK_SIZE; // sets stackPtr to the top of the newly allocated stack (stack grows down)

  Task* task = new Task(name, priority, stackBase, stackPtr, taskCode);
  taskList_.push_back(task); // for debugging purposes

  scheduler_->addToReadyList(task);

  updateNextFree();
}

void RTOS::startScheduler() {
  scheduler_->run();
}