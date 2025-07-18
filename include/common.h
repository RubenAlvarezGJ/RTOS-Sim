#ifndef COMMON_H
#define COMMON_H

/**
 * @typedef taskFunction_t
 * @brief Function pointer type for RTOS task functions.
 * 
 * Each task function takes a single void* argument (typically a TaskContext or nullptr in this simulation)
 * and returns nothing. This allows for flexible task definitions.
 */
typedef void (*taskFunction_t)(void* args );

#endif