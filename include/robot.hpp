#pragma once
#include <string>
#include <ostream>
class Task;

class Robot {
public:
    Robot(const std::string& id, const std::string& name, int battery);
    virtual ~Robot() = default;

    virtual void        work()  = 0;  // uses battery, sets status to "working"
    virtual void        charge();     // restores battery to 100
    virtual std::string type()  const = 0;

    std::string id()      const;
    std::string name()    const;
    int         battery() const;
    std::string status()  const;

    bool operator==(const Robot& other) const;

    // Returns e.g. "R2D2 + C3PO"
    std::string operator+(const Robot& other) const;

    // Why friend and not a regular method? Add a comment when you implement this.
    friend std::ostream& operator<<(std::ostream& os, const Robot& r);

protected:
    std::string id_;
    std::string name_;
    int         battery_;  // always clamped to [0, 100]
    std::string status_;   // "idle" | "working" | "charging"
};