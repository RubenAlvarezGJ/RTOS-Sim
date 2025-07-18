#ifndef CLOCK_H
#define CLOCK

#include <cstdint>

/**
 * @class Clock
 * @brief Simulates a system tick counter for the RTOS.
 * 
 * Used to control how long the RTOS runs and to simulate the passage of time.
 */
class Clock {
  public:
    Clock() = default;
    void tick() {tickCount_++;}
    std::size_t getTickCount() const {return tickCount_;};

  private:
    std::size_t tickCount_ = 0;
};

#endif 