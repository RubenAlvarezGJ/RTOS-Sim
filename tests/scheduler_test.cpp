#include <catch2/catch_test_macros.hpp>
#include "scheduler.h"
#include "task.h"
#include "config.h"
#include "output_control.h"

// Toggles the suppressOutput flag to true (see output_control.h if interested)
static SuppressOutputSetter _suppressOutputtSetter; 

// Simulated task functions for testing.
// These are passed into Task constructors to verify scheduler behavior.

void setFlagTrue(void* args) {
  bool* flag = static_cast<bool*>(args);
  *flag = true;
}

void logHighPriorityTask(void* args) {
  auto log = static_cast<std::vector<std::string>*>(args);
  log->push_back("High");
}

void logLowPriorityTask(void* args) {
  auto log = static_cast<std::vector<std::string>*>(args);
  log->push_back("Low");
}

TEST_CASE("Scheduler correctly runs a task") {
  uint8_t priority = 1;
  uint8_t stack[256];
  uint8_t* stackPtr = stack + 256;
  taskFunction_t taskCode = setFlagTrue;
  bool ran = false;

  Task task("Task", priority, stack, stackPtr, taskCode, &ran);
  Scheduler scheduler;

  scheduler.addToReadyList(&task);
  scheduler.run();

  REQUIRE(ran == true);
}

TEST_CASE("Scheduler doesn't run removed task") {
  uint8_t priority = 1;
  uint8_t stack[256];
  uint8_t* stackPtr = stack + 256;
  taskFunction_t taskCode = setFlagTrue;
  bool ran = false;

  Task task("Task", priority, stack, stackPtr, taskCode, &ran);
  Scheduler scheduler;

  scheduler.addToReadyList(&task);
  scheduler.removeTask(&task);
  scheduler.run();

  REQUIRE(ran == false);
}

TEST_CASE("Scheduler runs higher priority task first") {
  std::vector<std::string> log;
  uint8_t priority1 = 5, priority2 = 0;
  uint8_t stack1[256], stack2[256];
  uint8_t* stackPtr1 = stack1 + 256;
  uint8_t* stackPtr2 = stack2 + 256;
  taskFunction_t taskCode1 = logHighPriorityTask, taskCode2 = logLowPriorityTask;

  Task higherPri("High Priority", priority1, stack1, stackPtr1, taskCode1, &log);
  Task lowerPri("Low Priority", priority2, stack2, stackPtr2, taskCode2, &log);
  Scheduler scheduler;

  scheduler.addToReadyList(&higherPri);
  scheduler.addToReadyList(&lowerPri);
  scheduler.run();

  REQUIRE(log[0] == "High");
  REQUIRE(log[0 + configNUM_TASK_STEPS] == "Low");
}