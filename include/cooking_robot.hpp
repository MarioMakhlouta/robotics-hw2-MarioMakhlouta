#pragma once
#include "robot.hpp"

// CookingRobot inherits directly from Robot (not MobileRobot) — it doesn't
// move, it just cooks in place. This is the sibling branch shown in the
// class diagram: Robot -> CookingRobot, separate from the MobileRobot chain.
class CookingRobot : public Robot {
public:
    CookingRobot(const std::string& id, const std::string& name,
                int battery, const std::string& specialty);

    void        work()  override; // cooks a dish, uses 15% battery
    std::string type()  const override;

private:
    std::string specialty_; // e.g. "grilling", "baking", "sushi"
};