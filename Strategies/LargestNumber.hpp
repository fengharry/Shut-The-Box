#ifndef LARGEST_NUMBER_HPP
#define LARGEST_NUMBER_HPP

#include "Strategy.hpp"
using namespace std;


class LargestNumber: public Strategy {
    public:
        LargestNumber(uint32_t num_tiles_in=9): Strategy("Largest Number", "largest_number_" + to_string(num_tiles_in) + ".csv") {}
        LargestNumber(string csv_file_name_in): Strategy("Largest Number", csv_file_name_in) {}
        virtual void set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination,
            vector<vector<uint32_t>> &tile_combinations_dp) override;
};

#endif