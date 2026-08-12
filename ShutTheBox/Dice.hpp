#ifndef DICE_HPP
#define DICE_HPP
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

class Dice {
    protected:
        unordered_map<uint32_t, unordered_map<uint32_t, double>> probabilities;
        vector<double> single_die_probabilities_vector;
        unordered_map<uint32_t, bool> has_probabilities;
    public:
        /**
         * Constructs a dice object using the standard six-sided die.
         */
        Dice();

        /**
         * Constructs a dice object with its faces being the same as
         * faces_in, each with equal probability.
         * 
         * @param faces_in the faces of the die
         */
        Dice(unordered_set<uint32_t> faces_in);

        /**
         * Constructs a dice object with its faces and probabilities being
         * the (key, value) pairs in single_die_probabilities_in 
         * (faces = key, probabilities = value).
         * 
         * @param single_die_probabilities_in the probabilities of rolling
         *  each face of the die
         */
        Dice(unordered_map<uint32_t, double> single_die_probabilities_in);

        /**
         * Rolls 'num_dice' dice and returns the total sum of all the rolls.
         * 
         * @param num_dice the number of dice to roll
         * @return the total sum of dice rolls
         */
        uint32_t roll(uint32_t num_dice);

        /**
         * Sets the probabilities of rolling all possible values using 
         * 'num_dice' dice. If the probabilities have already been set 
         * (a.k.a. has_probabilities[num_dice] == true), this function 
         * does nothing. Otherwise, it calls set_probabilities(num_dice-1),
         * before then setting probabilities[num_dice].
         * 
         * @param num_dice the number of dice to roll
         */
        void set_probabilities(uint32_t num_dice);

        /**
         * Gets the probabilities of rolling 'roll_num' using 
         * 'num_dice' dice. If the probabilities have not been set 
         * (a.k.a. has_probabilities[num_dice] == false),
         * the function calls set_probabilities(num_dice-1).
         * 
         * @param roll_num the value of the dice roll
         * @param num_dice the number of dice that've been rolled
         * @return the probability of rolling 'roll_num' with 'num_dice' dice
         */
        double get_probability(uint32_t roll_num, uint32_t num_dice);

        /**
         * Gets all the possible values that can be rolled by
         * rolling any number of dice between 'num_dice_min' and 
         * 'num_dice_max' (inclusive). Returns a sorted vector
         * of all the rolls with no duplicates. 
         * If num_dice_min > num_dice_max, returns {};
         * 
         * @param num_dice_min the minimum number of dice to roll
         * @param num_dice_max the maximum number of dice to roll
         * @return a sorted vector of all possible values that can be rolled
         */
        vector<uint32_t> get_possible_rolls(uint32_t num_dice_min, uint32_t num_dice_max);

        /**
         * Sets 'possible_rolls' to be equal to the vector
         * described in get_possible_rolls. 
         * 
         * @param num_dice_min the minimum number of dice to roll
         * @param num_dice_max the maximum number of dice to roll
         * @param possible_rolls a reference to a vector of rolls
         */
        void set_to_possible_rolls(uint32_t num_dice_min, uint32_t num_dice_max, vector<uint32_t> &possible_rolls);
};

#endif