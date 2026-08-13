#include "SaveOne.hpp"
using namespace std;

void SaveOne::set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination,
vector<vector<uint32_t>> &tile_combinations_dp) {

    tile_combination.clear();

    uint32_t largest_tile = tile_combinations_dp.size() - 1;
    if (tile_combinations_dp[largest_tile][roll_num] == 0) return;

    vector<unordered_set<uint32_t>> tile_combinations;
    set_to_all_possible_tile_combinations(roll_num, tile_combinations, tile_combinations_dp);

    if (tile_combinations.size() == 0) return;
    else if (tile_combinations.size() == 1) {
        tile_combination = tile_combinations[0];
        return;
    }

    uint32_t max_tile = 0;
    
    for (unordered_set<uint32_t> &combination : tile_combinations) {
        if (combination.find(1) != combination.end()) continue;
        for (uint32_t tile : combination) {
            if (tile > max_tile) {
                max_tile = tile;
                tile_combination = combination;
            }
        }
    }
    if (max_tile > 0) return;

    for (unordered_set<uint32_t> &combination : tile_combinations) {
        for (uint32_t tile : combination) {
            if (tile > max_tile) {
                max_tile = tile;
                tile_combination = combination;
            }
        }
    }
}