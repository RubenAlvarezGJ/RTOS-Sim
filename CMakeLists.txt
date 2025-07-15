cmake_minimum_required(VERSION 3.11)
project(RTOS_Kernel_Simulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(rtos_kernel_sim ${SOURCES})

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)