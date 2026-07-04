#include "robot.hpp"
#include <algorithm> // std::clamp
#include <stdexcept>

Robot::Robot(const std::string& id, const std::string& name, int battery)
    : id_(id), name_(name),
    battery_(std::clamp(battery, 0, 100)), // clamp right in the constructor
    status_("idle") {}

void Robot::charge() {
    battery_ = 100;
    status_ = "idle";
}

std::string Robot::id()      const { return id_; }
std::string Robot::name()    const { return name_; }
int         Robot::battery() const { return battery_; }
std::string Robot::status()  const { return status_; }

void Robot::set_status(const std::string& status) {
    status_ = status;
}

bool Robot::operator==(const Robot& other) const {
    return id_ == other.id_;
}

std::string Robot::operator+(const Robot& other) const {
    return name_ + " + " + other.name_;
}

// friend, NOT a member function, because the left-hand side of << is
// std::ostream, not Robot. `robot << std::cout` would be the member
// syntax, which is backwards from how we actually want to write it
// (std::cout << robot). As a free function it has no access to
// Robot's protected members unless we declare it `friend` in the class.
std::ostream& operator<<(std::ostream& os, const Robot& r) {
    os << "[" << r.id_ << "] " << r.name_
    << " (" << r.type() << ") "
    << "battery=" << r.battery_ << "% status=" << r.status_;
    return os;
}