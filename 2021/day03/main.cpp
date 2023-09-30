#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>

constexpr int number_of_bits = 12;

bool get_file_contents(const std::string& file_name, std::vector<unsigned long long>& out_file_contents) {
    std::string current_line;

    std::ifstream input_file(file_name);
    if (!input_file.is_open()) {
        return false;
    }

    while (std::getline(input_file, current_line)) {
        std::bitset<number_of_bits> binary_number(current_line);
        out_file_contents.emplace_back( binary_number.to_ulong());
    }

    return true;
}

unsigned long long get_gamma_rate(const std::vector<unsigned long long>& file_contents) {
    int max_zeros;
    int max_ones;
    unsigned long long gamma_rate = 0;

    for (int i = number_of_bits - 1; i >= 0; i--) {

        max_zeros = 0;
        max_ones = 0;

        for (auto file_content : file_contents) {
            unsigned long long mask = 1ULL << i;
            if ((file_content & mask) == 0) {
                max_zeros++;
            } else {
                max_ones++;
            }
        }

        int bit_to_set = 0;

        if (max_ones > max_zeros) {
            bit_to_set = 1;
        }

        unsigned long long gamma_rate_mask = 1ULL << i;
        if (bit_to_set == 1) {
            gamma_rate = gamma_rate | gamma_rate_mask;
        } else {
            gamma_rate = gamma_rate & ~gamma_rate_mask;
        }
    }

    return gamma_rate;
}

unsigned long long get_epsilon_rate(unsigned long long gamma_rate) {
    std::bitset<number_of_bits> mask("111111111111");
    return gamma_rate ^ mask.to_ulong();
}

int main() {
    std::vector<unsigned long long> file_contents;
    bool parse_successful = get_file_contents("../input.txt", file_contents);
    if (!parse_successful) {
        std::cerr << "Parsing document was not successful" << std::endl;
        return 1;
    }

    unsigned long long gamma_rate = get_gamma_rate(file_contents);
    unsigned long long epsilon_rate = get_epsilon_rate(gamma_rate);
    unsigned long long power_consumption = gamma_rate * epsilon_rate;

    std::cout << "Gamma Rate: " << gamma_rate << std::endl;
    std::cout << "Epsilon Rate: " << epsilon_rate << std::endl;

    std::cout << "Power Consumption " << power_consumption << std::endl;

    return 0;
}
