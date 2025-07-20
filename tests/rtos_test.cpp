#include <catch2/catch_test_macros.hpp>
#include "rtos.h"
#include "task.h"
#include "context.h"
#include "config.h"
#include "output_control.h"

// Toggles the suppressOutput flag to true (see output_control.h if interested)
static SuppressOutputSetter _suppressOutputtSetter; 

// Used to log the execution of tasks.
static std::vector<std::string> tasksLog;

// Simulated task functions for testing.
// These are passed into createTask to verify task scheduling behavior.

void logTask(void* args) {
  auto ctx = static_cast<TaskContext*>(args);
  tasksLog.push_back(ctx->task->getName());
}

void logTaskWithDynamic(void* args) {
  auto ctx = static_cast<TaskContext*>(args);
  if (ctx->task->getName() == "Creator") {
    ctx->rtos->createTask("Dynamically Created Task", 2, logTaskWithDynamic);
  }
  tasksLog.push_back(ctx->task->getName());
}

TEST_CASE("Function executes correct number of times") {
  tasksLog.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("task", priority, taskCode);
  rtos.startScheduler();

  for (int i = 0; i < configNUM_TASK_STEPS; ++i) {
    REQUIRE(tasksLog[i] == "task");
  }

  REQUIRE(tasksLog.size() == configNUM_TASK_STEPS);
}

TEST_CASE("Function executes higher priority task first") {
  tasksLog.clear();

  std::size_t memSize = 1000;
  uint8_t priority1 = 5, priority2 = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("HigherPriority", priority1, taskCode);
  rtos.createTask("LowerPriority", priority2, taskCode);
  rtos.startScheduler();

  REQUIRE(tasksLog[0] == "HigherPriority");
  REQUIRE(tasksLog[0 + configNUM_TASK_STEPS] == "LowerPriority");
}

TEST_CASE("Round-robin scheduling amongst equal priority tasks") {
  tasksLog.clear();

  std::size_t memSize = 5000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTask;

  RTOS rtos(memSize);
  rtos.createTask("task1", priority, taskCode);
  rtos.createTask("task2", priority, taskCode);
  rtos.createTask("task3", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(tasksLog.size() == 3 * configNUM_TASK_STEPS); // each of the 3 tasks execute configNUM_TASK_STEPS times

  for (int i = 0; i < tasksLog.size(); ++i) {
    if (i % 3 == 0) {
      REQUIRE(tasksLog[i] == "task1");
    }
    else if (i % 3 == 1) {
      REQUIRE(tasksLog[i] == "task2");
    }
    else {
      REQUIRE(tasksLog[i] == "task3");
    }
  }
}

TEST_CASE("Tasks are able to be created dynamically") {
  tasksLog.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 5;
  taskFunction_t taskCode = logTaskWithDynamic;

  RTOS rtos(memSize);
  rtos.createTask("Creator", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(tasksLog[configNUM_TASK_STEPS] == "Dynamically Created Task");
}

TEST_CASE("Dynamically created task begins execution on the next clock tick if higher priority") {
  tasksLog.clear();

  std::size_t memSize = 1000;
  uint8_t priority = 1;
  taskFunction_t taskCode = logTaskWithDynamic;

  RTOS rtos(memSize);
  rtos.createTask("Creator", priority, taskCode);
  rtos.startScheduler();

  REQUIRE(tasksLog[0] == "Creator");
  REQUIRE(tasksLog[1] == "Dynamically Created Task");
  REQUIRE(tasksLog[1+configNUM_TASK_STEPS] == "Creator"); // original task resumes execution
}

TEST_CASE("Task creation fails when memory is insufficient") {
  tasksLog.clear();

  std::size_t memSize = configSTACK_SIZE - 1;

  RTOS rtos(memSize);
  rtos.createTask("ShouldFail", 1, logTask);
  rtos.startScheduler();

  REQUIRE(tasksLog.empty());
}

TEST_CASE("Task creation fails with invalid priority") {
  tasksLog.clear();

  std::size_t memSize = 1000;

  RTOS rtos(memSize);
  rtos.createTask("TooHigh", configMAX_PRIORITY + 1, logTask);
  rtos.createTask("TooLow", -1, logTask);
  rtos.startScheduler();

  REQUIRE(tasksLog.empty());
}

TEST_CASE("Scheduler runs with no tasks gracefully") {
  tasksLog.clear();

  RTOS rtos(1000);
  rtos.startScheduler();

  REQUIRE(tasksLog.empty()); // the idle task should run until the final clock tick
}

TEST_CASE("Creating maximum number of tasks that barely fit in memory") {
  tasksLog.clear();

  std::size_t memSize = configSTACK_SIZE * 3;

  RTOS rtos(memSize);
  rtos.createTask("T1", 1, logTask);
  rtos.createTask("T2", 1, logTask);
  rtos.createTask("T3", 1, logTask);

  rtos.createTask("T4", 1, logTask); // this one should fail
  rtos.startScheduler();

  REQUIRE(tasksLog.size() == 3 * configNUM_TASK_STEPS);
}