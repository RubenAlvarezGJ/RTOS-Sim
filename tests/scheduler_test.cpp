#include <catch2/catch_test_macros.hpp>
#include "scheduler.h"
#include "task.h"

/*
  These are helper functions used throughout the test cases.
  They are passed into Task object constructors as function pointers.
*/
void testFunction(void* args) {
  bool* returnVal = static_cast<bool*>(args);
  *returnVal = true;
}

void logHigherPriorityTask(void* args) {
  auto log = static_cast<std::vector<std::string>*>(args);
  log->push_back("Higher");
}

void logLowerPriorityTask(void* args) {
  auto log = static_cast<std::vector<std::string>*>(args);
  log->push_back("Lower");
}

TEST_CASE("Scheduler correctly runs a task") {
  uint8_t priority = 1;
  uint8_t stack[256];
  uint8_t* stackPtr = stack + 256;
  taskFunction_t taskFunction = testFunction;
  bool ran = false;

  Task task("Task", priority, stack, stackPtr, taskFunction, &ran);
  Scheduler scheduler;

  scheduler.addToReadyList(&task);
  scheduler.run();

  REQUIRE(ran == true);
}

TEST_CASE("Scheduler doesn't run removed task") {
  uint8_t priority = 1;
  uint8_t stack[256];
  uint8_t* stackPtr = stack + 256;
  taskFunction_t taskFunction = testFunction;
  bool ran = false;

  Task task("Task", priority, stack, stackPtr, taskFunction, &ran);
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
  taskFunction_t taskFunction1 = logHigherPriorityTask, taskFunction2 = logLowerPriorityTask;

  Task higherPri("higherPri", priority1, stack1, stackPtr1, taskFunction1, &log);
  Task lowerPri("lowerPri", priority2, stack2, stackPtr2, taskFunction2, &log);
  Scheduler scheduler;

  scheduler.addToReadyList(&higherPri);
  scheduler.addToReadyList(&lowerPri);
  scheduler.run();

  REQUIRE(log[0] == "Higher");
}