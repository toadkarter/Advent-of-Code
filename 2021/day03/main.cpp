#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <set>

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

void remove_from_set(std::set<unsigned long long>& current_candidates, const std::set<unsigned long long>& items_to_remove) {
    for (const auto item_to_remove: items_to_remove) {
        current_candidates.erase(item_to_remove);
    }
}

unsigned long long get_life_support_rating_values(const std::vector<unsigned long long>& file_contents, bool is_oxygen_generator) {

    int current_digit = number_of_bits - 1;

    std::set<unsigned long long> current_zeros;
    std::set<unsigned long long> current_ones;

    std::set<unsigned long long> current_candidates(
            std::make_move_iterator(file_contents.begin()),
            std::make_move_iterator(file_contents.end())
            );

    while (current_candidates.size() > 1 && current_digit >= 0) {
        current_zeros.clear();
        current_ones.clear();

        for (const auto current_candidate: current_candidates) {
            unsigned long long mask = 1ULL << current_digit;
            if ((current_candidate & mask) == 0) {
                current_zeros.emplace(current_candidate);
            } else {
                current_ones.emplace(current_candidate);
            }
        }

        if (is_oxygen_generator) {
            if (current_zeros.size() > current_ones.size()) {
                remove_from_set(current_candidates, current_ones);
            } else {
                remove_from_set(current_candidates, current_zeros);
            }
        } else {
            if (current_zeros.size() > current_ones.size()) {
                remove_from_set(current_candidates, current_zeros);
            } else {
                remove_from_set(current_candidates, current_ones);
            }
        }



        current_digit--;
    }

    if (current_candidates.size() == 1) {
        return *current_candidates.begin();
    } else {
        return -1;
    }
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
    unsigned long long mask = (1 << number_of_bits) - 1;
    return gamma_rate ^ mask;
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

    unsigned long long oxygen_generator = get_life_support_rating_values(file_contents, true);
    unsigned long long c02_scrubber = get_life_support_rating_values(file_contents, false);
    unsigned long long life_support_rating = oxygen_generator * c02_scrubber;

    std::cout << "Oxygen Generator: " << oxygen_generator << std::endl;
    std::cout << "C02 Scrubber: " << c02_scrubber << std::endl;

    std::cout << "Life Support Rating: " << life_support_rating << std::endl;

    return 0;
}
