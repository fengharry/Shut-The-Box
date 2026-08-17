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
 * Gets all possible combinations that sum to roll_num by applying
 * the following algorithm to 'tile_combinations_dp'.
 * 1. Each row's index represents the tile of that value (we call this
 *    value 'I') and all the tiles less than that value.
 * 2. Each column's index represents the total sum of the tiles
 *    (we call this value 'J').
 * 3. If the value at cell (I, J) > cell (I-1, J), then there
 *    must be a combination that includes Tile I.
 * 4. Assuming the cell (I, J) is included in the combination
 *    (Depends on the algorithm), step 3 will then be repeated for
 *    cell (I-1, J-I).
 * 5. The algorithm stops once I-1 = 0 or J-I = 0.
 *      - If the algorithm stops at J-I = 0, then it has reached a
 *        valid combination, and returns its unordered_set.
 *      - If it doesn't stop at J-I = 0, then it has reached an
 *        invalid combination, and returns {};
 * 
 * @param roll_num the total value that's been rolled by the dice
 * @param tile_combinations_dp a 2D vector described by 'ShutTheBox::get_tile_combinations_dp'.
 * @return an vector of unordered_sets containing all possible combinations
 */
vector<unordered_set<uint32_t>> get_all_possible_tile_combinations(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp);

/**
 * Sets tile_combinations to all possible combinations that sum to roll_num 
 * in the same way as described in 'get_all_possible_tile_combinations'.
 * 
 * @param roll_num the total value that's been rolled by the dice
 * @param tile_combinations a reference to a vector of unordered_sets
 * @param tile_combinations_dp a 2D vector described by 'ShutTheBox::get_tile_combinations_dp'.
 */
void set_to_all_possible_tile_combinations(const uint32_t roll_num, vector<unordered_set<uint32_t>> &tile_combinations,
            vector<vector<uint32_t>> &tile_combinations_dp);

#endif