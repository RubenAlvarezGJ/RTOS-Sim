#include <catch2/catch_test_macros.hpp>
#include "rtos.h"
#include "task.h"
#include "context.h"
#include "config.h"
#include "output_control.h"

// Toggles the suppressOutput flag to true (see output_control.h if interested)
static SuppressOutputSetter _suppressOutputtSetter; 

// Used to log the execution of tasks.
static std::vector<std::string> log;

// Simulated task functions for testing.
// These are passed into createTask to verify task scheduling behavior.

void logTask(void* args) {
  auto ctx = static_cast<TaskContext*>(args);
  log.push_back(ctx->task->getName());
}

void logTaskWithDynamic(void* args) {
  auto ctx = static_cast<TaskContext*>(args);
  if (ctx->task->getName() == "Creator") {
    ctx->rtos->createTask("Dynamically Created Task", 2, logTaskWithDynamic);
  }
  log.push_back(ctx->task->getName());
}

TEST_CASE("Function executes correct number of times") {
  log.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("task", priority, taskCode);
  rtos.startScheduler();

  for (int i = 0; i < configNUM_TASK_STEPS; ++i) {
    REQUIRE(log[i] == "task");
  }
}

TEST_CASE("Function executes higher priority task first") {
  log.clear();

  std::size_t memSize = 1000;
  uint8_t priority1 = 5, priority2 = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("HigherPriority", priority1, taskCode);
  rtos.createTask("LowerPriority", priority2, taskCode);
  rtos.startScheduler();

  REQUIRE(log[0] == "HigherPriority");
  REQUIRE(log[0 + configNUM_TASK_STEPS] == "LowerPriority");
}

TEST_CASE("Round-robin scheduling amongst equal priority tasks") {
  log.clear();

  std::size_t memSize = 5000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("task1", priority, taskCode);
  rtos.createTask("task2", priority, taskCode);
  rtos.createTask("task3", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(log.size() == 3 * configNUM_TASK_STEPS); // each of the 3 tasks execute configNUM_TASK_STEPS times

  for (int i = 0; i < log.size(); ++i) {
    if (i % 3 == 0) {
      REQUIRE(log[i] == "task1");
    }
    else if (i % 3 == 1) {
      REQUIRE(log[i] == "task2");
    }
    else {
      REQUIRE(log[i] == "task3");
    }
  }
}

TEST_CASE("Tasks are able to be created dynamically") {
  log.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 5;
  taskFunction_t taskCode = logTaskWithDynamic;

  RTOS rtos(memSize);
  rtos.createTask("Creator", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(log[configNUM_TASK_STEPS] == "Dynamically Created Task");
}

TEST_CASE("Dynamically created task begins execution on the next clock tick if higher priority") {
  log.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTaskWithDynamic;

  RTOS rtos(memSize);
  rtos.createTask("Creator", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(log[0] == "Creator");
  REQUIRE(log[1] == "Dynamically Created Task");
  REQUIRE(log[1+configNUM_TASK_STEPS] == "Creator"); // original task resumes execution
}

TEST_CASE("Task creation fails when memory is insufficient") {
  log.clear();

  std::size_t memSize = configSTACK_SIZE - 1;

  RTOS rtos(memSize);
  rtos.createTask("ShouldFail", 1, logTask);
  rtos.startScheduler();

  REQUIRE(log.empty());
}

TEST_CASE("Task creation fails with invalid priority") {
  log.clear();

  std::size_t memSize = 1000;

  RTOS rtos(memSize);
  rtos.createTask("TooHigh", configMAX_PRIORITY + 1, logTask);
  rtos.createTask("TooLow", -1, logTask);
  rtos.startScheduler();

  REQUIRE(log.empty());
}

TEST_CASE("Scheduler runs with no tasks gracefully") {
  log.clear();

  RTOS rtos(1000);
  rtos.startScheduler();

  REQUIRE(log.empty()); // the idle task should run until the final clock tick
}

TEST_CASE("Creating maximum number of tasks that barely fit in memory") {
  log.clear();

  std::size_t memSize = configSTACK_SIZE * 3;

  RTOS rtos(memSize);
  rtos.createTask("T1", 1, logTask);
  rtos.createTask("T2", 1, logTask);
  rtos.createTask("T3", 1, logTask);

  rtos.createTask("T4", 1, logTask); // this one should fail
  rtos.startScheduler();

  REQUIRE(log.size() == 3 * configNUM_TASK_STEPS);
}