#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <string>
#include "task.h"
#include "common.h"
#include "output_control.h"

static SuppressOutputSetter _suppressOutputtSetter; // Toggles the suppressIdleOutput flag to true

void dummyFunction(void* args) {
    return;
}

TEST_CASE("Tasks initialize correctly") {
    uint8_t dummyPriority = 4;
    uint8_t dummyStack[256];
    uint8_t* dummyStackPtr = dummyStack + 256;
    taskFunction_t taskFunction = dummyFunction;
    void* dummyArg = nullptr;

    Task dummyTask("Dummy Task", dummyPriority , dummyStack, dummyStackPtr, taskFunction, dummyArg);

    REQUIRE(dummyTask.getPriority() == dummyPriority);
    REQUIRE(dummyTask.getState() == Task::READY);
    REQUIRE(dummyTask.getStep() == 0);
    REQUIRE(dummyTask.getName() == "Dummy Task");
}