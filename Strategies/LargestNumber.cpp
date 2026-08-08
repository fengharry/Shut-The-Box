#include "LargestNumber.hpp"
using namespace std;

LargestNumber::LargestNumber(uint32_t num_tiles_in): ShutTheBox(num_tiles_in, "Largest Number", "../results/largest_number.csv") { 
}

LargestNumber::LargestNumber(unordered_set<uint32_t> tiles_in): ShutTheBox(tiles_in, "Largest Number", "../results/largest_number.csv") { 
}

LargestNumber::LargestNumber(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities_in): 
    ShutTheBox(tiles_in, dice_probabilities_in, "Largest Number", "../results/largest_number.csv") { 
}

void LargestNumber::set_combination(const uint32_t roll_num, unordered_set<uint32_t>& tile_combination) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_tile_combinations_dp(roll_num, tile_combinations_dp);

    if(tile_combinations_dp[largest_tile][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for(uint32_t i = largest_tile; i > 0; i--) {
        if (tile_combinations_dp[i-1][remaining_space] < tile_combinations_dp[i][remaining_space]) {
            remaining_space -= i;
            tile_combination.insert(i);
        }
    }
    if(remaining_space != 0) tile_combination.clear();
}