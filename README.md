# Robot Fleet Manager by Mario Makhlouta

A C++ console application simulating a small fleet of robots that can work,
charge, and be assigned tasks. Built for Session 2 of the inmind.ai Academy
Robotics training, practicing OOP (inheritance, polymorphism), smart pointers,
STL containers, operator overloading, threads, and exception handling.

## What it does

- Models a fleet of robots using an abstract `Robot` base class, with
  `MobileRobot` → `CleaningRobot` as a concrete inheritance chain.
- `Fleet` manages the robots (`std::shared_ptr<Robot>`, stored in an
  `std::unordered_map` for O(1) lookup by id) and a task queue
  (`std::priority_queue<Task>`, highest priority first).
- Robots can `work()` (draining battery, clamped to [0, 100]) and `charge()`
  (restoring to 100). `work()` throws `std::runtime_error` if battery is 0.
- `MobileRobot::start_work(seconds)` launches a background `std::thread` that
  calls `work()` once per second, safely stopped and joined via an
  `std::atomic<bool>` flag in the destructor.
- An interactive menu lets you add/remove robots, assign tasks, work/charge
  the fleet, and view status — with every risky operation wrapped in
  try/catch so bad input never crashes the program.

## Project structure

```
robotics-hw2-MarioMakhlouta/
├── include/        # all .hpp files
├── src/            # all .cpp files
|── main.cpp        # interactive menu
├── compile.sh      # compile command
├── README.md
├── ANSWERS.md
└── ENV_CHECK.md
```

## How to compile

```bash
chmod +x compile.sh
./compile.sh
```

This runs:
```bash
g++ -std=c++17 -Wall -Wextra -pthread src/*.cpp main.cpp -I include -o fleet_app
```

`-pthread` is required because `MobileRobot` uses `std::thread`.

## How to run

```bash
./fleet_app
```

You'll see a menu:
```
=== Robot Fleet Manager ===
1. Add robot
2. Remove robot
3. Show all robots
4. Work (single robot)
5. Work all
6. Charge all
7. Assign task to robot
8. Show task queue
9. Start timed work on a robot
0. Exit
```

## Design notes

- **`shared_ptr` in `Fleet`, not `unique_ptr`**: `Fleet::find()` needs to
  return a robot to the caller (e.g. `main.cpp`) while `Fleet` itself keeps
  ownership internally. `unique_ptr` only allows one owner at a time, so it
  can't be copied out like that — `shared_ptr` lets both `Fleet` and the
  caller safely hold a reference to the same robot.
- **`unordered_map` for `robots_`**: average O(1) lookup by id, which is the
  most frequent operation (`find`, `assign_task`, `remove`). Robot order
  doesn't matter for this fleet, so a hash map beats a vector (O(n) search).
- **`priority_queue` for the task queue**: built on a binary heap, giving
  O(log n) push/pop and O(1) access to the highest-priority task — exactly
  what "always process highest priority first" needs, without ever needing
  to search the middle of the queue.

## Design notes

- **`shared_ptr` in `Fleet`, not `unique_ptr`**: `Fleet::find()` needs to
  return a robot to the caller (e.g. `main.cpp`) while `Fleet` itself keeps
  ownership internally. `unique_ptr` only allows one owner at a time, so it
  can't be copied out like that — `shared_ptr` lets both `Fleet` and the
  caller safely hold a reference to the same robot.
- **`unordered_map` for `robots_`**: average O(1) lookup by id, which is the
  most frequent operation (`find`, `assign_task`, `remove`). Robot order
  doesn't matter for this fleet, so a hash map beats a vector (O(n) search).
- **`priority_queue` for the task queue**: built on a binary heap, giving
  O(log n) push/pop and O(1) access to the highest-priority task — exactly
  what "always process highest priority first" needs, without ever needing
  to search the middle of the queue.


## Problems I ran into

- **Duplicate robot ids silently overwrote existing robots.** While testing,
  I added a robot with id `1`, then later added a different robot also using
  id `1`. My original `Fleet::add()` just did `robots_[robot->id()] = robot;`,
  which silently replaced the first robot with no warning — it just vanished
  from `show all robots` with no error at all. I fixed this by checking if
  the id already existed in `add()` and throwing a `std::runtime_error` if
  so, which `main.cpp` already catches like every other risky call.

- **Verifying the background thread shuts down cleanly on exit.** Since
  `MobileRobot` launches a `std::thread` in `start_work()`, exiting the
  program while that thread is still running could crash with
  `terminate called without an active exception` if the thread isn't
  properly joined. I tested this directly by starting a long timed job and
  choosing "Exit" immediately while the robot's battery was still well above
  0 (so the thread was definitely still active). The program correctly
  waited for the thread's current cycle to finish and exited cleanly,
  confirming `stop_`/`worker_.join()` in the destructor work as intended
  even under real concurrent conditions.
