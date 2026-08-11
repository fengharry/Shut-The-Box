#include "MostNumbers.hpp"
using namespace std;

MostNumbers::MostNumbers(uint32_t num_tiles_in): ShutTheBox(num_tiles_in, "Most Numbers", "../results/most_numbers.csv") { 
}

MostNumbers::MostNumbers(unordered_set<uint32_t> tiles_in): ShutTheBox(tiles_in, "Most Numbers", "../results/most_numbers.csv") { 
}

MostNumbers::MostNumbers(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, double> single_die_probabilities_in): 
    ShutTheBox(tiles_in, single_die_probabilities_in, "Most Numbers", "../results/most_numbers.csv") { 
}

void MostNumbers::set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination) {
    tile_combination.clear();
    vector<vector<uint32_t>> tile_combinations_dp;
    set_to_tile_combinations_dp(roll_num, tile_combinations_dp);

    if(tile_combinations_dp[largest_tile][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for(uint32_t i = largest_tile; i > 0; i--) {
        if (tile_combinations_dp[i-1][remaining_space] == 0 && tile_combinations_dp[i][remaining_space] > 0) {
            remaining_space -= i;
            tile_combination.insert(i);
        }
    }
    if(remaining_space != 0) tile_combination.clear();
}