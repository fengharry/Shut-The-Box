#include "Dice.hpp"
using namespace std;

std::random_device dev;
std::mt19937 rng(dev());

Dice::Dice() {
    for (uint32_t roll_num = 0; roll_num <= 6; roll_num++) {
        if (roll_num == 0) {
            single_die_probabilities_vector.push_back(0);
        }
        else {
            single_die_probabilities_vector.push_back(1.0/6.0);
            probabilities[1][roll_num] = 1.0/6.0;
        }
    }
    smallest_roll = 1;
    has_probabilities[1] = true;
}

Dice::Dice(unordered_set<uint32_t> single_die_faces_in) {
    uint32_t vector_size = 0;
    for (const uint32_t roll_num : single_die_faces_in) {
        if (roll_num > vector_size) vector_size = roll_num;
        if (roll_num < smallest_roll) smallest_roll = roll_num;
    }

    single_die_probabilities_vector = vector<double>(vector_size, 0);
    for (const auto &roll_num : single_die_faces_in) {
        single_die_probabilities_vector[roll_num] = 1.0/single_die_faces_in.size();
        probabilities[1][roll_num] = 1.0/single_die_faces_in.size();
    }
    has_probabilities[1] = true;
}

Dice::Dice(unordered_map<uint32_t, double> single_die_probabilities_in) {
    probabilities[1] = single_die_probabilities_in;

    uint32_t vector_size = 0;
    for (const auto &probability : single_die_probabilities_in) {
        if (probability.first > vector_size) vector_size = probability.first;
        if (probability.first < smallest_roll) smallest_roll = probability.first;
    }
    single_die_probabilities_vector = vector<double>(vector_size, 0);
    for (const auto &probability : single_die_probabilities_in) {
        single_die_probabilities_vector[probability.first] = probability.second;
    }
    has_probabilities[1] = true;
}

uint32_t Dice::get_smallest_roll() { return smallest_roll; }

uint32_t Dice::roll(uint32_t num_dice) {
    std::discrete_distribution<std::mt19937::result_type> dice_roll(single_die_probabilities_vector.begin(), single_die_probabilities_vector.end());
    uint32_t sum = 0;
    for (uint32_t curr_dice = 0; curr_dice < num_dice; curr_dice++) {
        sum += dice_roll(rng);
    }
    return sum;
}

void Dice::set_probabilities(uint32_t num_dice) {
    if (num_dice <= 1 || has_probabilities[num_dice]) return;
    else set_probabilities(num_dice-1);

    for (const auto &prev_dice : probabilities[num_dice-1]) {
        const uint32_t prev_roll_num = prev_dice.first;
        const double prev_probability = prev_dice.second;
        for (const auto &single_die : probabilities[1]) {
            const uint32_t single_roll_num = single_die.first;
            const double single_probability = single_die.second;

            probabilities[num_dice][prev_roll_num + single_roll_num] += prev_probability * single_probability;
        }
    }
    has_probabilities[num_dice] = true;
}

double Dice::get_probability(uint32_t roll_num, uint32_t num_dice) {
    if (!has_probabilities[num_dice]) set_probabilities(num_dice);
    return probabilities[num_dice][roll_num];
}

vector<uint32_t> Dice::get_possible_rolls(uint32_t num_dice_min, uint32_t num_dice_max) {
    vector<uint32_t> possible_rolls;
    set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
    return possible_rolls;
}

void Dice::set_to_possible_rolls(uint32_t num_dice_min, uint32_t num_dice_max, vector<uint32_t> &possible_rolls) {
    possible_rolls.clear();
    if (!has_probabilities[num_dice_min]) set_probabilities(num_dice_min);
    if (num_dice_min == num_dice_max) {
        for (const auto roll : probabilities[num_dice_min]) {
            possible_rolls.push_back(roll.first);
        }
    } else {
        unordered_set<uint32_t> rolls_set;
        for (uint32_t num_dice = num_dice_min; num_dice <= num_dice_max; num_dice++) {
            if (!has_probabilities[num_dice]) set_probabilities(num_dice);
            for (const auto roll : probabilities[num_dice]) {
                rolls_set.insert(roll.first);
            }
        }
        for (uint32_t roll : rolls_set) {
            possible_rolls.push_back(roll);
        }
    }
    sort(possible_rolls.begin(), possible_rolls.end());
}