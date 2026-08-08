#ifndef MOST_NUMBERS_HPP
#define MOST_NUMBERS_HPP

#include "ShutTheBox.hpp"
using namespace std;


class MostNumbers: public ShutTheBox {
    public:
        MostNumbers(uint32_t num_tiles_in=9);
        MostNumbers(unordered_set<uint32_t> tiles_in);
        MostNumbers(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities_in);

        virtual void set_combination(const uint32_t roll_num, unordered_set<uint32_t>& tile_combination) override;
};

#endif