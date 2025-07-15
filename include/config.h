#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

constexpr std::size_t configSTACK_SIZE = 256;      // 256 byte stack by default, change at your own risk
constexpr std::size_t configMAX_RAM_SIZE = 16382;  // maximum allowed simulated RAM size (16 KB), change at your own risk
constexpr std::size_t configMAX_PRIORITY = 5;      // maximum allowed priority level
constexpr std::size_t configMAX_TICKS = 50;        // maximum simulated time (might change later to allow for non-determinisitic run time)
constexpr std::size_t configNUM_TASK_STEPS = 5;    // number of steps a task must complete before it's marked as complete

#endif