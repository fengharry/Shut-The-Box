#include "Strategy.hpp"
using namespace std;

Strategy::Strategy(string strategy_name_in, string strategy_csv_file_name_in): 
    strategy_name(strategy_name_in), strategy_csv_file_name(strategy_csv_file_name_in) {}

string Strategy::get_name() { return strategy_name; }

string Strategy::get_csv_file_name() { return strategy_csv_file_name; }

void Strategy::set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination, 
    vector<vector<uint32_t>> &tile_combinations_dp) { }