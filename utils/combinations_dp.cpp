#include "combinations_dp.hpp"
using namespace std;

vector<unordered_set<uint32_t>> get_all_possible_tile_combinations(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp) {
    vector<unordered_set<uint32_t>> tile_combinations;
    set_to_all_possible_tile_combinations(roll_num, tile_combinations, tile_combinations_dp);
    return tile_combinations;
}

void set_to_all_possible_tile_combinations(const uint32_t roll_num, vector<unordered_set<uint32_t>> &tile_combinations,
vector<vector<uint32_t>> &tile_combinations_dp) {
    tile_combinations.clear();

    uint32_t largest_tile = tile_combinations_dp.size() - 1;
    if (tile_combinations_dp[largest_tile][roll_num] == 0) return;

    queue<CombinationPath> combinations_queue;
    
    for (uint32_t i = largest_tile; i > 0; i--) {
        if (tile_combinations_dp[i-1][roll_num] < tile_combinations_dp[i][roll_num]) {
            combinations_queue.push({i-1, roll_num - i, {i}});
        }
    }

    while (combinations_queue.size() > 0) {
        CombinationPath c = combinations_queue.front();
        combinations_queue.pop();
        if (c.remaining_space <= 0) {
            tile_combinations.push_back(c.numbers);
            continue;
        }
        for (uint32_t i = c.start_idx; i > 0; i--) {
            if (tile_combinations_dp[i-1][c.remaining_space] < tile_combinations_dp[i][c.remaining_space]) {
                c.numbers.insert(i);
                combinations_queue.push({i-1, c.remaining_space - i, c.numbers});
                c.numbers.erase(i);
            }
        }
    }
}