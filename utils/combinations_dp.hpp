#ifndef COMBINATIONS_DP_HPP
#define COMBINATIONS_DP_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stdint.h>
#include <queue>
using namespace std;

struct CombinationPath {
    uint32_t start_idx;
    uint32_t remaining_space;
    unordered_set<uint32_t> numbers;
};

/**
 * Gets all possible combinations that sum to roll_num in the same way
 * as described in get_strategy_combination.
 * 
 * @param roll_num the total value that's been rolled by the dice
 * @return an vector of unordered_sets containing all possible combinations
 */
vector<unordered_set<uint32_t>> get_all_possible_tile_combinations(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp);

/**
 * Sets tile_combinations to all possible combinations that sum to roll_num 
 * in the same way as described in get_strategy_combination.
 * 
 * @param roll_num the total value that's been rolled by the dice
 * @param tile_combinations a reference to a vector of unordered_sets
 */
void set_to_all_possible_tile_combinations(const uint32_t roll_num, vector<unordered_set<uint32_t>> &tile_combinations,
            vector<vector<uint32_t>> &tile_combinations_dp);

#endif