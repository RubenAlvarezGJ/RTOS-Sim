cmake_minimum_required(VERSION 3.11)
project(RTOS_Kernel_Simulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)

file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")
add_library(rtos_lib ${SOURCES})

# Main application
add_executable(rtos_kernel_sim app/main.cpp)
target_link_libraries(rtos_kernel_sim PRIVATE rtos_lib)

# Test executable
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE rtos_lib Catch2::Catch2WithMain)