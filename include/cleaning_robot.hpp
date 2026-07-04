#pragma once
#include "mobile_robot.hpp"

class CleaningRobot : public MobileRobot {
public:
    CleaningRobot(const std::string& id, const std::string& name,
                int battery, double speed, int bin_capacity);

    void        work() override; // moves AND cleans, uses more battery
    std::string type() const override;

private:
    int bin_capacity_;
};