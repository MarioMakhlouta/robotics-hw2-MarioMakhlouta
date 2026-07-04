#include "task.hpp"

// priority_queue<Task> is a MAX-heap by default: top() returns the
// "largest" element according to operator<. So if we define
// "smaller priority number = smaller task", the highest priority
// number naturally ends up on top. No need to reverse anything.
bool Task::operator<(const Task& other) const {
    return priority < other.priority;
}

std::ostream& operator<<(std::ostream& os, const Task& t) {
    os << "[" << t.name << "] priority=" << t.priority
    << " assigned_to=" << t.assigned_to;
    return os;
}