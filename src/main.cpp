#include "fleet.hpp"
#include "mobile_robot.hpp"
#include "cleaning_robot.hpp"
#include <iostream>
#include <limits>

int read_int(const std::string& prompt) {
    std::cout << prompt;
    int value;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a valid number: ";
    }
    return value;
}

int main() {
    Fleet fleet;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n=== Robot Fleet Manager ===\n"
                    "1. Add robot\n2. Remove robot\n3. Show all robots\n"
                    "4. Work (single robot)\n5. Work all\n6. Charge all\n"
                    "7. Assign task to robot\n8. Show task queue\n"
                    "9. Start timed work on a robot\n0. Exit\n";
        choice = read_int("Choose: ");

        try {
            switch (choice) {
                case 1: {
                    std::cout << "id name battery speed: ";
                    std::string id, name; int battery; double speed;
                    std::cin >> id >> name >> battery >> speed;
                    fleet.add(std::make_shared<MobileRobot>(id, name, battery, speed));
                    break;
                }
                case 2: {
                    std::cout << "id to remove: ";
                    std::string id; std::cin >> id;
                    fleet -= id; // exercises operator-=
                    break;
                }
                case 3:
                    std::cout << fleet;
                    break;
                case 4: {
                    std::cout << "id: ";
                    std::string id; std::cin >> id;
                    fleet.find(id)->work(); // throws if missing OR battery empty
                    break;
                }
                case 5: fleet.work_all(); break;
                case 6: fleet.charge_all(); break;
                case 7: {
                    std::cout << "robot id, task name, priority(1-5): ";
                    std::string id, name; int priority;
                    std::cin >> id >> name >> priority;
                    fleet.assign_task(id, Task{name, priority, id});
                    break;
                }
                case 8: fleet.show_tasks(); break;
                case 9: {
                    std::cout << "id, seconds: ";
                    std::string id; int seconds;
                    std::cin >> id >> seconds;
                    auto robot = fleet.find(id);
                    if (auto mobile = std::dynamic_pointer_cast<MobileRobot>(robot)) {
                        mobile->start_work(seconds);
                    } else {
                        std::cout << "That robot type can't do timed work.\n";
                    }
                    break;
                }
                case 0: std::cout << "Bye!\n"; break;
                default: std::cout << "Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n"; // catches EVERY throwing path above
        }
    }
}