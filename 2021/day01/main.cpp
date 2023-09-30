#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

bool get_file_contents(const std::string& file_name, std::vector<int>& out_file_contents) {
    std::string current_line;

    std::ifstream input_file(file_name);
    if (!input_file.is_open()) {
        return false;
    }

    while (std::getline(input_file, current_line)) {
        try {
            int current_number = std::stoi(current_line);
            out_file_contents.push_back(current_number);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument passed into std::stoi: " << e.what() << std::endl;
            return false;
        }
    }

    return true;
}

int get_single_window_increases(const std::vector<int>& file_contents) {
    int total_increases = 0;

    for (int i = 1; i < file_contents.size(); i++) {
        if (file_contents[i] > file_contents[i - 1]) {
            total_increases++;
        }
    }

    return total_increases;
}

int get_triple_window_increases(const std::vector<int>& file_contents) {
    int total_increases = 0;

    int left_pointer = 0;
    int right_pointer = 0;

    int previous_window_sum = std::numeric_limits<int>::max();
    int current_window_sum = 0;

    for (; right_pointer < file_contents.size(); right_pointer++) {
        current_window_sum += file_contents[right_pointer];

        if (right_pointer - left_pointer < 2) {
            continue;
        }

        if (right_pointer > 2) {
            current_window_sum -= file_contents[left_pointer];
            left_pointer++;
        }

        if (current_window_sum > previous_window_sum) {
            total_increases++;
        }

        previous_window_sum = current_window_sum;
    }

    return total_increases;
}

int main() {
    std::vector<int> file_contents;

    bool parse_successful = get_file_contents("../input.txt", file_contents);
    if (!parse_successful) {
        std::cerr << "Parsing document was not successful" << std::endl;
        return 1;
    }

    int total_increases_single_window = get_single_window_increases(file_contents);
    std::cout << "The number of increases in a single window is " << total_increases_single_window << std::endl;

    int total_increases_triple_window = get_triple_window_increases(file_contents);
    std::cout << "The number of increases in a triple window is " << total_increases_triple_window << std::endl;

    return 0;
}
