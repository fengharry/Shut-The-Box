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
using namespace std;

class Strategy {
    protected:
        string strategy_name = "";
        string strategy_csv_file_name = "";
    public:
        Strategy(string strategy_name_in, string strategy_csv_file_name_in="");
        string get_name();
        string get_csv_file_name();
        
        /**
         * Sets tile_combination to the combination that fits the strategy
         * of the current class. This function will vary from child to child, 
         * However, the general algorithm remains the same:
         * 1. 'tile_combinations_dp" is the 2D vector described by 
         *    get_tile_combinations_dp from the ShutTheBox object. 
         * 2. Each row's index represents the tile of that value (we call this
         *    value 'I') and all the tiles less than that value.
         * 3. Each column's index represents the total sum of the tiles
         *    (we call this value 'J').
         * 4. If the value at cell (I, J) > cell (I-1, J), then there
         *    must be a combination that includes Tile I.
         * 5. Assuming the cell (I, J) is included in the combination
         *    (Depends on the algorithm), step 3 will then be repeated for
         *    cell (I-1, J-I).
         * 6. The algorithm stops once I-1 = 0 or J-I = 0.
         *      - If the algorithm stops at J-I = 0, then it has reached a
         *        valid combination, and returns its unordered_set.
         *      - If it doesn't stop at J-I = 0, then it has reached an
         *        invalid combination, and returns {};
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @param tile_combination a reference to an unordered_set
         */
        virtual void set_to_combination(const uint32_t roll_num, unordered_set<uint32_t> &tile_combination, 
            vector<vector<uint32_t>> &tile_combinations_dp);
};

#endif