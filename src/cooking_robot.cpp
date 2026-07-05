#include "cooking_robot.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

CookingRobot::CookingRobot(const std::string& id, const std::string& name,
                            int battery, const std::string& specialty)
    : Robot(id, name, battery), specialty_(specialty) {}

void CookingRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error(name_ + " cannot work: battery is empty");
    }
    battery_ = std::clamp(battery_ - 15, 0, 100); // cooking costs 15%
    status_ = "working";
    std::cout << name_ << " is cooking up some " << specialty_
            << "... smells great!\n";
}

std::string CookingRobot::type() const { return "CookingRobot"; }