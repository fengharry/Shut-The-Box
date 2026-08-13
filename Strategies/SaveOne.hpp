#ifndef SAVE_ONE_HPP
#define SAVE_ONE_HPP

#include "Strategy.hpp"
using namespace std;


class SaveOne: public Strategy {
    public:
        SaveOne(uint32_t num_tiles_in=9): Strategy("Save One") {}
        virtual void set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination,
            vector<vector<uint32_t>> &tile_combinations_dp) override;
};

#endif