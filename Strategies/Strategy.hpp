#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stdint.h>
#include <queue>
#include "../utils/all_combinations.hpp"
using namespace std;

class Strategy {
    protected:
        string strategy_name = "";
    public:
        Strategy(string strategy_name_in);
        string get_name();
        
        /**
         * Sets 'tile_combination' to a unordered set of tiles, using
         * the following algorithm on 'tile_combinations_dp' to do so.
         * 1. Each row's index represents the tile of that value (we call this
         *    value 'I') and all the tiles less than that value.
         * 2. Each column's index represents the total sum of the tiles
         *    (we call this value 'J').
         * 3. If the value at cell (I, J) > cell (I-1, J), then there
         *    must be a combination that includes Tile I.
         * 4. Assuming the cell (I, J) is included in the combination
         *    (Depends on the algorithm), step 3 will then be repeated for
         *    cell (I-1, J-I).
         * 5. The algorithm stops once I-1 = 0 or J-I = 0.
         *      - If the algorithm stops at J-I = 0, then it has reached a
         *        valid combination, and returns its unordered_set.
         *      - If it doesn't stop at J-I = 0, then it has reached an
         *        invalid combination, and returns {};
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @param tile_combinations_dp a 2D vector described by 'ShutTheBox::get_tile_combinations_dp'.
         * @return an vector of unordered_sets containing all possible combinations
         */
        virtual void set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination, 
            vector<vector<uint32_t>> &tile_combinations_dp);
};

#endif