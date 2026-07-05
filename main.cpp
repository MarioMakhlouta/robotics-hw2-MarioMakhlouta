#include "fleet.hpp"
#include "mobile_robot.hpp"
#include "cleaning_robot.hpp"
#include "cooking_robot.hpp"
#include <iostream>
#include <limits>
#include <cctype>   // for std::isdigit, std::isspace

// ---- Validated input helpers ----
// Used everywhere user input is read, so no path can leave std::cin in a
// broken state or leak leftover characters into the next prompt.

int read_int(const std::string& prompt, bool allow_negative = false) {
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number. ";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!allow_negative && value < 0) {
            std::cout << "Value cannot be negative. ";
            continue;
        }
        return value;
    }
}
double read_double(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number. ";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < 0) {
            std::cout << "Value cannot be negative. ";
            continue;
        }
        return value;
    }
}

std::string read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

int main() {
    Fleet fleet;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\033[34m\n=== Robot Fleet Manager ===\n"
                    "1. Add robot\n2. Remove robot\n3. Show all robots\n"
                    "4. Work (single robot)\n5. Work all\n6. Charge all\n"
                    "7. Assign task to robot\n8. Show task queue\n"
                    "9. Start timed work on a robot\n0. Exit\n\033[0m";
        choice = read_int("Choose: ");

        try {
            switch (choice) {
                case 1: {
                    std::cout << "\033[33m\nWhat kind of robot are you building today?\n"
                                "  1. MobileRobot   - a general-purpose wanderer\n"
                                "  2. CleaningRobot - scrubs floors, empties its bin\n"
                                "  3. CookingRobot  - whips up meals in the kitchen\n\033[0m";
                    int type_choice = read_int("Pick a type (1-3): ");

                    std::string id = read_line("id: ");
                    std::string name = read_line("name: ");
                    int battery = read_int("battery: ");

                    switch (type_choice) {
                        case 1: {
                            double speed = read_double("speed (m/s): ");
                            fleet.add(std::make_shared<MobileRobot>(id, name, battery, speed));
                            std::cout << "🤖 " << name << " rolls off the assembly line, ready to roam!\n";
                            break;
                        }
                        case 2: {
                            double speed = read_double("speed (m/s): ");
                            int bin_capacity = read_int("bin capacity (L): ");
                            fleet.add(std::make_shared<CleaningRobot>(id, name, battery, speed, bin_capacity));
                            std::cout << "🧹 " << name << " gleams with fresh polish, dust bin at the ready!\n";
                            break;
                        }
                        case 3: {
                            std::string specialty = read_line("specialty (e.g. grilling, baking, sushi): ");
                            fleet.add(std::make_shared<CookingRobot>(id, name, battery, specialty));
                            std::cout << "👨‍🍳 " << name << " ties on an apron, eager to cook some " << specialty << "!\n";
                            break;
                        }
                        default:
                            std::cout << "Unknown robot type — nothing was created.\n";
                    }
                    break;
                }
                case 2: {
                    std::string id = read_line("id to remove: ");
                    fleet -= id; // exercises operator-=
                    break;
                }
                case 3:
                    std::cout << fleet;
                    break;
                case 4: {
                    std::string id = read_line("id: ");
                    fleet.find(id)->work(); // throws if missing OR battery empty
                    break;
                }
                case 5: fleet.work_all(); break;
                case 6: fleet.charge_all(); break;
                case 7: {
                    std::string id = read_line("robot id: ");
                    std::string name = read_line("task name: ");
                    int priority = read_int("priority (1-5): ");
                    fleet.assign_task(id, Task{name, priority, id});
                    break;
                }
                case 8: fleet.show_tasks(); break;
                case 9: {
                    std::string id = read_line("id: ");
                    int seconds = read_int("seconds: ");
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