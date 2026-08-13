#ifndef LARGEST_NUMBER_HPP
#define LARGEST_NUMBER_HPP

#include "Strategy.hpp"
using namespace std;


class LargestNumber: public Strategy {
    public:
        LargestNumber(): Strategy("Largest Number") {}
        virtual void set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination,
            vector<vector<uint32_t>> &tile_combinations_dp) override;
};

#endif