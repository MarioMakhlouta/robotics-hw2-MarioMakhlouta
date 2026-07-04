#include "fleet.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

void Fleet::add(std::shared_ptr<Robot> robot) {
    if (robots_.find(robot->id()) != robots_.end()) {
        throw std::runtime_error("A robot with id " + robot->id() + " already exists");
    }
    robots_[robot->id()] = robot;
}

void Fleet::remove(const std::string& id) {
    robots_.erase(id);
}

std::shared_ptr<Robot> Fleet::find(const std::string& id) const {
    auto it = robots_.find(id);
    if (it == robots_.end()) {
        throw std::runtime_error("No robot with id: " + id);
    }
    return it->second;
}

// void Fleet::assign_task(const std::string& robot_id, const Task& t) {
//     auto robot = find(robot_id); // throws if not found — propagates up
//     Task assigned = t;
//     assigned.assigned_to = robot_id;
//     tasks_.push(assigned);
// }

void Fleet::assign_task(const std::string& robot_id, const Task& t) {
    auto robot = find(robot_id); // throws if not found
    Task assigned = t;
    assigned.assigned_to = robot_id;
    tasks_.push(assigned);

    // Mark the robot busy now that it has a pending task.
    // We use a public setter rather than making Fleet a friend of Robot,
    // because a setter keeps the door open for validation later
    // (e.g. rejecting an invalid status string) without Fleet needing
    // to know Robot's internals.
    robot->set_status("working");
}

void Fleet::show_tasks() const {
    auto copy = tasks_; // priority_queue has no iterator, so we copy to print
    while (!copy.empty()) {
        std::cout << copy.top() << "\n";
        copy.pop();
    }
}

void Fleet::work_all() const {
    for (const auto& [id, robot] : robots_) {
        try {
            robot->work();
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << "\n"; // one bad robot
        }                                                // doesn't stop the rest
    }
}

void Fleet::charge_all() const {
    for (const auto& [id, robot] : robots_) {
        robot->charge();
    }
}

std::size_t Fleet::size()  const { return robots_.size(); }
bool        Fleet::empty() const { return robots_.empty(); }

Fleet& Fleet::operator+=(std::shared_ptr<Robot> robot) {
    add(robot);
    return *this;
}

Fleet& Fleet::operator-=(const std::string& id) {
    remove(id);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Fleet& f) {
    for (const auto& [id, robot] : f.robots_) {
        os << *robot << "\n";
    }
    return os;
}

std::shared_ptr<Robot> Fleet::find_low_battery(int threshold) const {
    auto it = std::find_if(robots_.begin(), robots_.end(),
        [threshold](const auto& pair) {
            return pair.second->battery() < threshold;
        });
    return (it != robots_.end()) ? it->second : nullptr;
}