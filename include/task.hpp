#pragma once
#include <string>
#include <ostream>

// Task is assigned to a robot through Fleet::assign_task()
// and stored in the task queue inside Fleet.
// Create Task objects in main and pass them to the fleet.
struct Task {
    std::string name;
    int         priority;    // 1 (low) to 5 (high)
    std::string assigned_to; // robot id

    // Used by the task queue to order tasks — higher priority comes first
    bool operator<(const Task& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Task& t);
};