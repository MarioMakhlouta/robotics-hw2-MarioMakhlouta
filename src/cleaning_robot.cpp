#include "cleaning_robot.hpp"
#include <algorithm> //for std::clamp
#include <iostream>
#include <stdexcept>

CleaningRobot::CleaningRobot(const std::string& id, const std::string& name,
                            int battery, double speed, int bin_capacity)
    : MobileRobot(id, name, battery, speed), bin_capacity_(bin_capacity) {
    if (bin_capacity_ < 0) {
        throw std::invalid_argument("Bin capacity cannot be negative");
    }
}

void CleaningRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error(name_ + " cannot work: battery is empty");
    }
    battery_ = std::clamp(battery_ - 25, 0, 100); // cleaning drains a bit more
    status_ = "working";
    std::cout << name_ << " is cleaning (bin capacity " << bin_capacity_ << "L)\n";
}

std::string CleaningRobot::type() const { return "CleaningRobot"; }