#include "mobile_robot.hpp"
#include <algorithm> // for std::clamp
#include <chrono>
#include <iostream>
#include <stdexcept>

MobileRobot::MobileRobot(const std::string& id, const std::string& name,
                        int battery, double speed)
    : Robot(id, name, battery), speed_(speed) {}

MobileRobot::~MobileRobot() {
    stop_ = true;
    if (worker_.joinable()) worker_.join();
}

void MobileRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error(name_ + " cannot work: battery is empty");
    }
    battery_ = std::clamp(battery_ - 20, 0, 100);
    status_ = "working";
    std::cout << name_ << " is moving at " << speed_ << " m/s\n";
}

std::string MobileRobot::type() const { return "MobileRobot"; }

void MobileRobot::start_work(int seconds) {
    stop_ = false; // reset in case start_work is called more than once
    worker_ = std::thread([this, seconds]() {
        for (int i = 0; i < seconds && !stop_; ++i) {
            try {
                work();
            } catch (const std::runtime_error& e) {
                std::cout << "Error: " << e.what() << "\n";
                break; // battery empty -> stop the loop early
            }
            std::cout << name_ << " | battery=" << battery_
                    << " status=" << status_ << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        status_ = "idle";
    });
}