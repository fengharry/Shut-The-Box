#ifndef DIE_HPP
#define DIE_HPP
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Die {
    protected:
        string name = "D6";
        uint32_t smallest_roll;
        uint32_t largest_roll;

    public:

        unordered_map<uint32_t, double> probabilities;
        vector<double> probabilities_vector;

        /**
         * Constructs a dice object using the standard six-sided die.
         */
        Die(string name_in="D6");

        /**
         * Constructs a dice object with its faces being the same as
         * faces_in, each with equal probability.
         * 
         * @param faces_in the faces of the die
         */
        Die(unordered_set<uint32_t> faces_in);

        /**
         * Constructs a dice object with its faces and probabilities being
         * the (key, value) pairs in single_die_probabilities_in 
         * (faces = key, probabilities = value).
         * 
         * @param single_die_probabilities_in the probabilities of rolling
         *  each face of the die
         */
        Die(unordered_map<uint32_t, double> single_die_probabilities_in);

        string get_name();

        uint32_t get_smallest_roll();

        uint32_t get_largest_roll();

        /**
         * Rolls 'num_dice' dice and returns the total sum of all the rolls.
         * 
         * @param num_dice the number of dice to roll
         * @return the total sum of dice rolls
         */
        uint32_t roll();
};

#endif