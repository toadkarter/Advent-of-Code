#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

enum Direction {
    Forward,
    Down,
    Up,
    None,
};

typedef std::pair<Direction, int> Command;

Direction get_direction(const std::string& direction_literal) {
    if (direction_literal == "forward") {
        return Forward;
    } else if (direction_literal == "down") {
        return Down;
    } else if (direction_literal == "up") {
        return Up;
    } else {
        return None;
    }
}

bool get_file_contents(const std::string& file_name, std::vector<Command>& out_file_contents) {
    std::string current_line;

    std::ifstream input_file(file_name);
    if (!input_file.is_open()) {
        return false;
    }

    while (std::getline(input_file, current_line)) {
        std::string direction;
        int amount;

        std::istringstream iss(current_line);
        if (iss >> direction >> amount) {
            out_file_contents.emplace_back(get_direction(direction), amount);
        } else {
            std::cerr << "Could not parse string into pair" << std::endl;
        }
    }

    return true;
}

int get_position(const std::vector<Command>& file_contents) {
    int current_x = 0;
    int current_y = 0;
    int aim = 0;

    for (const Command& current_command: file_contents) {
        switch (current_command.first) {
            case Forward:
                current_x += current_command.second;
                current_y += (current_command.second * aim);
                break;
            case Down:
                aim += current_command.second;
                break;
            case Up:
                aim = std::max(0, aim - current_command.second);
                break;
            case None:
                break;
        }
    }

    return current_x * current_y;
}

int main() {
    std::vector<Command> file_contents;
    bool parse_successful = get_file_contents("../input.txt", file_contents);
    if (!parse_successful) {
        std::cerr << "Parsing document was not successful" << std::endl;
        return 1;
    }

    int final_position = get_position(file_contents);

    std::cout << "The final position is: " << final_position << std::endl;

    return 0;
}
