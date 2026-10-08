#ifndef DICE_HPP
#define DICE_HPP
#include "Die.hpp"
using namespace std;

class Dice {
    protected:
        // unordered_map<uint32_t, unordered_map<uint32_t, double>> probabilities;
        // vector<double> single_die_probabilities_vector;
        // unordered_map<uint32_t, bool> has_probabilities;
        // unordered_map<uint32_t, uint32_t> smallest_rolls;
        // unordered_map<uint32_t, uint32_t> largest_rolls;

        vector<Die> dice;
        vector<string> all_dice_names;
        unordered_map<string, uint32_t> dice_idx;

        unordered_map<string, unordered_map<uint32_t, double>> m_probabilities;
        vector<vector<double>> single_die_probability_vectors;
        unordered_map<string, bool> m_has_probabilities;
        unordered_map<string, uint32_t> m_smallest_rolls;
        unordered_map<string, uint32_t> m_largest_rolls;

        struct DiceQueueElement {
            uint32_t total_value;
            double probability;
        };

    public:

        string dice_names_to_key(const vector<string> &dice_names);

        /**
         * Constructs a dice object using the standard six-sided die.
         */
        Dice();

        /**
         * Constructs a dice object, adds the die 'die_in' via 'add_die(die_in)'.
         * 
         * @param die_in a Die object
         */
        Dice(Die die_in);

        /**
         * Adds the die 'die_in' to 'dice', records it's index in 'dice_idx'.
         * If 'die_in' is already recorded, this function does nothing.
         * 
         * @param die_in a Die object
         */
        void add_die(Die die_in);

        const vector<string> &get_all_dice_names() const;

        uint32_t get_num_dice() const;

        uint32_t get_smallest_roll(const vector<string> &dice_names);

        uint32_t get_largest_roll(const vector<string> &dice_names);

        /**
         * Rolls 'num_dice' dice and returns the total sum of all the rolls.
         * 
         * @param num_dice the number of dice to roll
         * @return the total sum of dice rolls
         */
        uint32_t roll(const vector<string> &dice_names);

        /**
         * Sets the probabilities of rolling all possible values using 
         * 'num_dice' dice. If the probabilities have already been set 
         * (a.k.a. has_probabilities[num_dice] == true), this function 
         * does nothing. Otherwise, it calls set_probabilities(num_dice-1),
         * before then setting probabilities[num_dice].
         * 
         * @param num_dice the number of dice to roll
         */
        void set_probabilities(const vector<string> &dice_names);

        void set_probabilities_step(queue<DiceQueueElement> &roll_queue, const vector<string> &dice_names, uint32_t name_idx);

        /**
         * Gets the probabilities of rolling 'roll_num' using 
         * 'num_dice' dice. If the probabilities have not been set 
         * (a.k.a. has_probabilities[num_dice] == false),
         * the function calls set_probabilities(num_dice - 1).
         * 
         * @param roll_num the value of the dice roll
         * @param num_dice the number of dice that've been rolled
         * @return the probability of rolling 'roll_num' with 'num_dice' dice
         */
        double get_probability(uint32_t roll_num, const vector<string> &dice_names);

        /**
         * Gets all the possible values that can be rolled by
         * rolling any number of dice between 'num_dice_min' and 
         * 'num_dice_max' (inclusive). Returns a sorted vector
         * of all the rolls with no duplicates. 
         * If (num_dice_min > num_dice_max), returns {};
         * 
         * @param num_dice_min the minimum number of dice to roll
         * @param num_dice_max the maximum number of dice to roll
         * @return a sorted vector of all possible values that can be rolled
         */
        vector<uint32_t> get_possible_rolls(const vector<string> &dice_names, bool include_subsets=false);

        vector<uint32_t> get_possible_rolls(bool include_subsets=false);

        /**
         * Sets 'possible_rolls' to be equal to the vector
         * described in 'get_possible_rolls'. 
         * 
         * @param num_dice_min the minimum number of dice to roll
         * @param num_dice_max the maximum number of dice to roll
         * @param possible_rolls a reference to a vector of rolls
         */
        void set_to_possible_rolls(const vector<string> &dice_names, vector<uint32_t> &possible_rolls, bool include_subsets=false);

        void set_to_possible_rolls(vector<uint32_t> &possible_rolls, bool include_subsets=false);
};

#endif