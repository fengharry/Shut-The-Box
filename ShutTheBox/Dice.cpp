#include <queue>
#include "Dice.hpp"
using namespace std;

void subsetRecur(int i, const vector<string>& arr, 
       vector<vector<string>>& res, vector<string>& subset) {
    
    // add subset at end of array
    if (i == arr.size()) {
        if (subset.size() > 0) res.push_back(subset);
        return;
    }
    
    // include the current value and 
    // recursively find all subsets
    subset.push_back(arr.at(i));
    subsetRecur(i+1, arr, res, subset);
    
    // exclude the current value and 
    // recursively find all subsets.
    subset.pop_back();
    subsetRecur(i+1, arr, res, subset);
}

vector<vector<string>> subsets(const vector<string>& arr) {
    
    vector<string> subset;
    vector<vector<string>> res;
    
    // finding recursively
    subsetRecur(0, arr, res, subset);
    return res;
}

string Dice::dice_names_to_key(const vector<string> &dice_names) {
    vector<string> names_copy(dice_names);
    sort(names_copy.begin(), names_copy.end());
    string key = "";
    for (const string &name : names_copy) {
        key += name + "\n";
    }
    return key;
}

Dice::Dice() {
    add_die(Die());
    add_die(Die());
}

Dice::Dice(Die die_in) {
    add_die(die_in);
    add_die(die_in);
}

void Dice::add_die(Die die_in) {
    if (all_dice_names.size() == 0) {
        dice.push_back(die_in);
        dice_idx[die_in.get_name()] = 0;
    } else {
        auto dice_idx_ptr = dice_idx.find(die_in.get_name());
        if (dice_idx_ptr == dice_idx.end()) {
            dice.push_back(die_in);
            dice_idx_ptr->second = dice.size() - 1;
        }
    }
    all_dice_names.push_back(die_in.get_name());
}

const vector<string> &Dice::get_all_dice_names() const {
    return all_dice_names;
}

uint32_t Dice::get_num_dice() const {
    return all_dice_names.size();
}

uint32_t Dice::get_smallest_roll(const vector<string> &dice_names) { 
    string key = dice_names_to_key(dice_names);
    if (m_has_probabilities[key]) set_probabilities(dice_names);
    return m_smallest_rolls[key];
}
uint32_t Dice::get_largest_roll(const vector<string> &dice_names) { 
    string key = dice_names_to_key(dice_names);
    if (m_has_probabilities[key]) set_probabilities(dice_names);
    return m_largest_rolls[key];
}

uint32_t Dice::roll(const vector<string> &dice_names) {
    uint32_t total_roll = 0;
    for (const string &name : dice_names) {
        Die &die = dice[dice_idx[name]];
        total_roll += die.roll();
    }
    return total_roll;
}

void Dice::set_probabilities(const vector<string> &dice_names) {
    string key = dice_names_to_key(dice_names);
    if (m_has_probabilities[key]) return;

    m_smallest_rolls[key] = INT_MAX;
    m_largest_rolls[key] = 0;

    queue<DiceQueueElement> roll_queue;
    set_probabilities_step(roll_queue, dice_names, 0);

    while (!roll_queue.empty()) {
        DiceQueueElement roll = roll_queue.front();
        roll_queue.pop();

        if (roll.total_value < m_smallest_rolls[key]) m_smallest_rolls[key] = roll.total_value;
        if (roll.total_value > m_largest_rolls[key]) m_largest_rolls[key] = roll.total_value;
        m_probabilities[key][roll.total_value] += roll.probability;
    }
    m_has_probabilities[key] = true;
}

void Dice::set_probabilities_step(queue<DiceQueueElement> &roll_queue, const vector<string> &dice_names, uint32_t name_idx) {
    if(name_idx >= dice_names.size()) return; 

    Die &curr_die = dice[dice_idx[dice_names[name_idx]]];

    if (name_idx == 0) {
        for (auto &it : curr_die.probabilities) {
            const uint32_t &roll_num = it.first;
            const double &roll_probability = it.second;
            roll_queue.push({roll_num, roll_probability});
        }
    } else {
        uint32_t size = roll_queue.size();
        for (uint32_t i = 0; i < size; ++i) {
            DiceQueueElement curr_roll = roll_queue.front();
            roll_queue.pop();

            for (auto &it : curr_die.probabilities) {
                const uint32_t &roll_num = it.first;
                const double &roll_probability = it.second;
                roll_queue.push({curr_roll.total_value + roll_num, curr_roll.probability * roll_probability});
            }
        }
    }
    set_probabilities_step(roll_queue, dice_names, name_idx+1);

}

double Dice::get_probability(uint32_t roll_num, const vector<string> &dice_names) {
    string key = dice_names_to_key(dice_names);
    if (!m_has_probabilities[key]) set_probabilities(dice_names);
    return m_probabilities[key][roll_num];
}

vector<uint32_t> Dice::get_possible_rolls(const vector<string> &dice_names, bool include_subsets) {
    vector<uint32_t> possible_rolls;
    set_to_possible_rolls(dice_names, possible_rolls, include_subsets);
    return possible_rolls;
}

vector<uint32_t> Dice::get_possible_rolls(bool include_subsets) {
    vector<uint32_t> possible_rolls;
    set_to_possible_rolls(all_dice_names, possible_rolls, include_subsets);
    return possible_rolls;
}

void Dice::set_to_possible_rolls(const vector<string> &dice_names, vector<uint32_t> &possible_rolls, bool include_subsets) {
    possible_rolls.clear();

    if(include_subsets) {
        unordered_set<uint32_t> rolls_set;
        vector<vector<string>> dice_combos = subsets(dice_names);

        for (const vector<string> &dice_combo : dice_combos) {
            string key = dice_names_to_key(dice_combo);
            if (!m_has_probabilities[key]) set_probabilities(dice_combo);

            for (const auto roll_prob : m_probabilities[key]) {
                rolls_set.insert(roll_prob.first);
            }
        }
        for (const auto roll : rolls_set) {
            possible_rolls.push_back(roll);
        }
    } else {
        string key = dice_names_to_key(dice_names);
        if (!m_has_probabilities[key]) set_probabilities(dice_names);
        
        for (const auto roll_prob : m_probabilities[key]) {
            possible_rolls.push_back(roll_prob.first);
        }
    }
    sort(possible_rolls.begin(), possible_rolls.end());
}

void Dice::set_to_possible_rolls(vector<uint32_t> &possible_rolls, bool include_subsets) {
    set_to_possible_rolls(all_dice_names, possible_rolls, include_subsets);
}