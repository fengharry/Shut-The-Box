#include "MostNumbers.hpp"
using namespace std;

void MostNumbers::set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination,
vector<vector<uint32_t>> &tile_combinations_dp) {

    uint32_t largest_tile = tile_combinations_dp.size() - 1;

    if (tile_combinations_dp[largest_tile][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for (uint32_t i = largest_tile; i > 0; i--) {
        if (tile_combinations_dp[i-1][remaining_space] == 0 && tile_combinations_dp[i][remaining_space] > 0) {
            remaining_space -= i;
            tile_combination.insert(i);
        }
    }
    if (remaining_space != 0) tile_combination.clear();
}