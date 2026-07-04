#pragma once
#include "robot.hpp"
#include <thread>
#include <atomic>

class MobileRobot : public Robot {
public:
    MobileRobot(const std::string& id, const std::string& name,
                int battery, double speed);

    ~MobileRobot() override; //added by me to ensure proper cleanup of the thread
    
    void        work()  override;  // moves, uses 20% battery
    std::string type()  const override;

    // Launches a background thread that calls work() once per second
    // for the given duration, printing status after each call.
    // stop_ and worker_ are yours to use -- figure out how.
    void start_work(int seconds);

protected:
    double            speed_;
    std::thread       worker_;
    std::atomic<bool> stop_{ false };
};