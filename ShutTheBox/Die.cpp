#include "Die.hpp"
using namespace std;

std::random_device dev;
std::mt19937 rng(dev());

Die::Die(string name_in) {
    for (uint32_t roll_num = 0; roll_num <= 6; roll_num++) {
        if (roll_num == 0) {
            probabilities_vector.push_back(0);
        }
        else {
            probabilities_vector.push_back(1.0/6.0);
            probabilities[roll_num] = 1.0/6.0;
        }
    }
    smallest_roll = 1;
    largest_roll = 6;
}

Die::Die(unordered_set<uint32_t> faces_in) {
    uint32_t vector_size = 0;
    for (const uint32_t roll_num : faces_in) {
        if (roll_num > vector_size) vector_size = roll_num;
        if (roll_num < smallest_roll) smallest_roll = roll_num;
        if (roll_num > largest_roll) largest_roll = roll_num;
    }

    probabilities_vector = vector<double>(vector_size, 0);
    for (const auto &roll_num : faces_in) {
        probabilities_vector[roll_num] = 1.0/faces_in.size();
        probabilities[roll_num] = 1.0/faces_in.size();
    }
}

Die::Die(unordered_map<uint32_t, double> probabilities_in) {
    probabilities = probabilities_in;

    uint32_t vector_size = 0;
    for (const auto &probability : probabilities_in) {
        if (probability.first > vector_size) vector_size = probability.first;
        if (probability.first < smallest_roll) smallest_roll = probability.first;
        if (probability.first > largest_roll) largest_roll = probability.first;
    }
    probabilities_vector = vector<double>(vector_size, 0);
    for (const auto &probability : probabilities_in) {
        probabilities_vector[probability.first] = probability.second;
    }
}

string Die::get_name() {
    return name;
}

uint32_t Die::get_smallest_roll() { 
    return smallest_roll;
}
uint32_t Die::get_largest_roll() { 
    return largest_roll;
}

uint32_t Die::roll() {
    std::discrete_distribution<std::mt19937::result_type> die_roll(probabilities_vector.begin(), probabilities_vector.end());
    return die_roll(rng);
}

