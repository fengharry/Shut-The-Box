#include "LargestNumber.hpp"
using namespace std;

void LargestNumber::set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination, 
vector<vector<uint32_t>> &tile_combinations_dp) {
    
    uint32_t largest_tile = tile_combinations_dp.size() - 1;

    if (tile_combinations_dp[largest_tile][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for (uint32_t tile = largest_tile; tile > 0; tile--) {
        if (tile_combinations_dp[tile-1][remaining_space] < tile_combinations_dp[tile][remaining_space]) {
            remaining_space -= tile;
            tile_combination.insert(tile);
        }
    }
    if (remaining_space != 0) tile_combination.clear();
}