#pragma once
#include "robot.hpp"
#include "task.hpp"
#include <memory>
#include <string>
#include <ostream>

#include <unordered_map> 
#include <queue>

class Fleet {
public:
    void add(std::shared_ptr<Robot> robot);
    void remove(const std::string& id);
    std::shared_ptr<Robot> find(const std::string& id) const;

    // Stores the task in the priority queue and marks the robot as busy.
    void assign_task(const std::string& robot_id, const Task& t);
    void show_tasks()  const;  // prints the task queue in priority order

    void work_all()    const;  // calls work() on every robot
    void charge_all()  const;  // calls charge() on every robot

    std::size_t size()  const;
    bool        empty() const;

    Fleet& operator+=(std::shared_ptr<Robot> robot);
    Fleet& operator-=(const std::string& id);
    friend std::ostream& operator<<(std::ostream& os, const Fleet& f);

    std::shared_ptr<Robot> find_low_battery(int threshold) const; //added by Mario Makhlouta

private:
    // Choose the right containers and justify each choice in a comment.
    // Hint: robots need fast lookup by id.
    // Hint: tasks must always come out highest priority first.


    // unordered_map: average O(1) lookup by id (hash table).
    // We look robots up by id constantly (find, assign_task, remove),
    // and robot order doesn't matter for the fleet, so a hash map
    // beats a vector (O(n) search) or a plain map (O(log n)).
    std::unordered_map<std::string, std::shared_ptr<Robot>> robots_;

    // priority_queue: built on a binary heap over a vector internally.
    // We only ever need "give me the highest priority task next" —
    // push and pop are O(log n), and top() is O(1). We never need to
    // search the middle of the queue, so a heap is the right fit
    // (a sorted vector would need O(n) inserts to stay sorted).
    std::priority_queue<Task> tasks_;
};