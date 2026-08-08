#ifndef LARGEST_NUMBER_HPP
#define LARGEST_NUMBER_HPP

#include "ShutTheBox.hpp"
using namespace std;


class LargestNumber: public ShutTheBox {
    public:
        LargestNumber(uint32_t num_tiles_in=9);
        LargestNumber(unordered_set<uint32_t> tiles_in);
        LargestNumber(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities_in);

        virtual void set_combination(const uint32_t roll_num, unordered_set<uint32_t>& tile_combination) override;
};

#endif