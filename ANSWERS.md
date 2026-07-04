# ANSWERS.md

### 1. Why shared_ptr in Fleet and not unique_ptr? What would break if you switched?

I used `shared_ptr` because sometimes I need to take a robot out of the Fleet
(like with `find()`) and use it outside, while the Fleet still keeps it too.
With `unique_ptr` only one owner is allowed at a time, so if `find()` returned
it, the Fleet would lose ownership or I'd have to return a raw pointer, which
is less safe. If I switched to `unique_ptr`, `find()` couldn't just return a
copy anymore, and I'd have more trouble managing who "owns" the robot.

### 2. What container did you use for robots_? Why — what is its lookup cost?

I used `std::unordered_map<std::string, std::shared_ptr<Robot>>`. I look up
robots by id a lot (find, remove, assign_task), so I wanted fast lookup.
`unordered_map` gives average O(1) lookup because it's a hash table. Order of
robots doesn't matter for me, so I didn't need a `map` (which is O(log n)) or
a `vector` (which would be O(n) to search).

### 3. What container did you use for the task queue? What data structure is it built on, and why is it the right fit?

I used `std::priority_queue<Task>`. It's built on a heap (a binary heap
internally, using a vector). It fits because I always need the highest
priority task to come out first, and a heap gives me that in O(log n) for
push/pop, and O(1) to just look at the top one. I don't need to search
through the middle of the queue, so a heap is a good fit instead of sorting
a whole list every time.

### 4. operator<< is declared friend inside Robot. What does friend give it that a regular method does not? Why can it not be a regular member function?

`friend` lets a function that is NOT a member of the class still access its
private/protected members. It has to be a free function (not a member)
because of how `<<` works: `std::cout << robot` actually means
`operator<<(std::cout, robot)`. The left side is `std::cout`, not `Robot`, so
it can't be written as a method inside `Robot` (that would only work if we
wrote it backwards like `robot << std::cout`, which is wrong). Since it can't
be a member, but it still needs to read Robot's protected fields to print
them, I made it a `friend` function instead.

### 5. You used std::find_if with a lambda. What does that communicate compared to a hand-written loop?

Using `find_if` with a lambda tells the reader right away "I'm searching for
something that matches a condition" without them needing to read the whole
loop to figure out what it's doing. A hand-written loop with an if inside
would do the same job, but you'd have to read every line to understand the
purpose. `find_if` basically says the intent in one line.

### 6. work() throws if battery is 0. Why use an exception rather than returning false or printing an error?

If I just returned `false` or printed something, whoever calls `work()`
could easily ignore it and keep going like nothing happened. With an
exception, the caller is forced to either handle it or let it pass up, it
can't just be silently skipped. Also the exception carries a message
(`.what()`) that explains exactly why it failed, not just a plain `false`
with no information.

### 7. An exception is thrown inside assign_task when the robot is not found. Is the fleet's state consistent afterwards?

Yes. In my code, `find()` is called first and throws right away if the robot
id doesn't exist, before anything else happens. Since the task is only
pushed to the queue after `find()` succeeds, nothing gets added to the queue
if the robot doesn't exist. So the Fleet stays in a clean, consistent state
even when the exception happens.

### 8. start_work uses a background thread. What did you do with stop_ and worker_, and why?

`worker_` holds the actual thread that runs `work()` once per second in a
loop. `stop_` is an atomic flag I check inside that loop — as long as it's
`false`, the loop keeps going. In the destructor of `MobileRobot`, I set
`stop_ = true` and then call `worker_.join()`. This makes sure the thread
actually finishes before the object gets destroyed. If I didn't do this, the
thread could still be running when the object is destroyed, and the program
would crash with `std::terminate()`.

### 9. Diamond inheritance

```cpp
class Robot { public: std::string name_ = "base"; };
class MobileRobot      : public Robot {};
class CookingRobot     : public Robot {};
class MaintenanceRobot : public MobileRobot, public CookingRobot {};
```

This doesn't compile because `MaintenanceRobot` inherits from both
`MobileRobot` and `CookingRobot`, and each of them has its own separate copy
of `Robot` inside them. So `MaintenanceRobot` actually contains **two**
copies of `Robot` — one through `MobileRobot`, one through `CookingRobot`.
When I write `m.name_`, the compiler doesn't know which of the two `name_`
copies I mean, so it's ambiguous and refuses to compile.

The fix is `virtual` inheritance:

```cpp
class Robot { public: std::string name_ = "base"; };
class MobileRobot      : public virtual Robot {};
class CookingRobot     : public virtual Robot {};
class MaintenanceRobot : public MobileRobot, public CookingRobot {};

int main() {
    MaintenanceRobot m;
    std::cout << m.name_; // now works
}
```

Adding `virtual` to the inheritance (not to a function, to the inheritance
itself) tells the compiler that if this class gets inherited more than once
through a diamond shape, only keep ONE shared copy instead of duplicating
it. So now `MobileRobot` and `CookingRobot` both point to the same single
`Robot`, and `m.name_` is no longer ambiguous.