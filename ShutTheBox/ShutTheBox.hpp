#ifndef SHUT_THE_BOX_HPP
#define SHUT_THE_BOX_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include "Dice.hpp"
#include "../Strategies/StrategyList.hpp"
#include "../utils/combinations_dp.hpp"

using namespace std;

enum OptimizedType {
    WIN_PROBABILITY, AVERAGE_SCORE
};

struct Results {
    double win_probability = 0;
    double avg_score = 0;
    unordered_map<uint32_t, unordered_set<uint32_t>> next_tile_combinations;
    unordered_map<uint32_t, double> next_win_probabilities;
    unordered_map<uint32_t, double> next_avg_scores;
};

class ShutTheBox {
    protected:
        string optimal_win_csv_file = "";
        string optimal_score_csv_file = "";
        
        Results empty_results = {1.0, 0};

        unordered_set<string> all_positions;
        vector<uint32_t> sorted_tiles;
        uint32_t largest_tile = 0;
        uint32_t tile_sum = 0;

        unordered_set<uint32_t> tiles;
        Dice dice;
        uint32_t num_dice_max = 2;
        uint32_t num_dice_min = 1;

        uint32_t initial_score = 0;
        uint32_t num_face_up = 0;
        unordered_map<uint32_t, bool> is_tile_face_up;

        /**
         * Rolls two dice, returns the sum of their face-up faces
         *
         * @return The number rolled by the two dice
         */
        uint32_t roll_double();

        /**
         * Rolls a dice, returns it's face-up face
         *
         * @return The number rolled by the die
         */
        uint32_t roll_single();
        
        /**
         * Inserts all possible positions with the given tiles into all_positions. 
         * Each position is a string with a space separating each face up tile
         * (see get_curr_position for more details on formatting).
         * 
         * @param tile_idx the index of the currently tracked tile in sorted_tiles
         * @param curr_position the string representing the current position,
         *      may be incomplete (ex. "1 3 4").
         */
        void get_all_positions(uint32_t tile_idx=0, string curr_position="");

        /**
         * Given a set of positions that have been reached by a simulation,
         * finds all positions that haven't been reached and records them
         * in a csv file.
         * 
         * @param csv_out the output stream of the csv file
         * @param reached_positions an unordered set of valid positions
         */
        void csv_record_unreachable_positions(std::ostream &csv_out, unordered_map<string, Results> &reached_positions);

        /**
         * Records a given position and its results in a csv file.
         * 
         * @param csv_out the output stream of the csv file
         * @param curr_position the current position
         * @param results the results of that position (i.e. win_probability, avg_score, etc.)
         */
        void csv_record_position(std::ostream &csv_out, string curr_position, Results &results);

    public:

        /**
         * Resets this object's tiles back to face_up
         */
        virtual void initialize_game();

        /**
         * Resets the tiles in face_up_tiles_in back to face_up.
         * If a tile is not a valid tile for the object,
         * this function will ignore that tile.
         * 
         * @param face_up_tiles_in the set of face up tiles
         */
        virtual void initialize_game(unordered_set<uint32_t> face_up_tiles_in);

        /**
         * Constructs a ShutTheBox object. Inserts numbers
         * from 1 to num_tiles_in into tiles and sorted_tiles, as well as
         * copying optimal_win_csv_file_in into optimal_win_csv_file
         * and optimal_score_csv_file_in into optimal_score_csv_file.
         * Also, gets all positions (i.e. runs get_all_positions()),
         * and initializes the game (i.e. runs initialize_game()).
         * 
         * @param num_tiles_in the number of tiles inserted (numbered from 1 to num_tiles_in)
         * @param optimal_win_csv_file_in the path of the object's win probability-optimized csv file
         * @param optimal_score_csv_file_in the path of the object's score-optimized csv file
         */
        ShutTheBox(uint32_t num_tiles_in=9, string optimal_win_csv_file_in="", string optimal_score_csv_file_in="");

        /**
         * Constructs a ShutTheBox object.
         * Copies tiles_in into tiles, strategy_name_in into strategy_name,
         * optimal_win_csv_file_in into optimal_win_csv_file,
         * and optimal_score_csv_file_in into optimal_score_csv_file.
         * Also, gets all positions (i.e. runs get_all_positions())
         * and initializes the game (i.e. runs initialize_game()).
         * 
         * @param tiles_in the set of tiles to be inserted
         * @param optimal_win_csv_file_in the path of the object's win probability-optimized csv file
         * @param optimal_score_csv_file_in the path of the object's score-optimized csv file
         */
        ShutTheBox(unordered_set<uint32_t> tiles_in, string optimal_win_csv_file_in="", string optimal_score_csv_file_in="");

        /**
         * Constructs a ShutTheBox object.
         * Copies tiles_in into tiles, strategy_name_in into strategy_name,
         * optimal_win_csv_file_in into optimal_win_csv_file,
         * and optimal_score_csv_file_in into optimal_score_csv_file.
         * Also, gets all positions (i.e. runs get_all_positions()),
         * constructs dice based on single_die_probabilities_in,
         * and initializes the game (i.e. runs initialize_game()).
         * 
         * @param tiles_in the set of tiles to be inserted
         * @param single_die_probabilities_in the probabilities of rolling 
         *  select values on a single die
         * @param optimal_win_csv_file_in the path of the object's win probability-optimized csv file
         * @param optimal_score_csv_file_in the path of the object's score-optimized csv file
         */
        ShutTheBox(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, double> single_die_probabilities_in, 
            string optimal_win_csv_file_in="", string optimal_score_csv_file_in="");
        
        /**
         * Gets the current position of the game as a string.
         * ex. "1 3 4 5 8 9"
         * 
         * @return a string of the current position
         */
        string get_curr_position();

        /**
         * Sets the string 'position' to the current position of the game.
         * 
         * @param position a reference to the position string
         */
        void set_to_curr_position(string &position);
        
        /**
         * Gets a 2D vector of size (largest_tile + 1, roll_num + 1)
         * calculated by Dynamic Programming, the algorithm for which is as follows:
         * 1. The cells at column 0 and/or row 0 are all equal to 0.
         * 2. Each row's index represents the tile of that value (we call this
         *    value "I") and all the tiles less than that value.
         * 3. Each column's index represents the total sum of the tiles
         *    (we call this value "J").
         * 4. The current cell (I, J) will be set equal to cell (I-1, J) if...
         *      - Tile I is not a valid tile in the object
         *      - Tile I is valid and J < I.
         * 5. If Tile I is valid and J >= I, then we set the current cell equal to
         *    cell (I-1, J) + cell (I-1, J-I).
         * It's worth noting that the value at cell (largest_tile, roll_num) is
         * equal to the number of valid tile combinations that sum to roll_num.
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @return a 2D vector as described above
         */
        vector<vector<uint32_t>> get_tile_combinations_dp(const uint32_t roll_num);

        /**
         * Sets a reference to a 2D vector to be a vector of size 
         * (largest_tile + 1, roll_num + 1) calculated by Dynamic Programming.
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @param tile_combinations_dp a reference to a 2D vector
         */
        void set_to_tile_combinations_dp(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp);

        /**
         * Gets the number of tile combinations that will sum to roll_num.
         * This is done by returning the cell (largest_tile, roll_num)
         * from the 2D vector as described by get_tile_combinations_dp;
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @return the number of tile combinations for that roll_num
         */
        uint32_t get_num_combinations(const uint32_t roll_num);
        
        /**
         * Gets the combination that fits the strategy of the 
         * current class. This function, or more specifically set_to_strategy_combination,
         * will vary from child to child, and will use the 2D vector 
         * as described by get_tile_combinations_dp. However, the general
         * algorithm for obtaining a combination remains the same:
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
         * @return an unordered_set of the chosen tile combination
         */
        unordered_set<uint32_t> get_strategy_combination(Strategy *strategy, const uint32_t roll_num);

        /**
         * Sets tile_combination to the combination as described in get_strategy_combination.
         * 
         * @param roll_num the total value that's been rolled by the dice
         * @param tile_combination a reference to an unordered_set
         */
        virtual void set_to_strategy_combination(Strategy *strategy, const uint32_t roll_num, unordered_set<uint32_t> &tile_combination);

        /**
         * Gets a random sequence of numbers that represents the values that
         * the object's dice can roll in a game. The sequence ends once 
         * the sum of values >= tile_sum-1. It's worth noting that
         * if this sum is equal to tile_sum-1, then a single die roll is added
         * instead of a double dice roll. This function, or more specifically
         * set_to_generated_sequence, will vary from child to child, depending on
         * how many dice should be rolled each turn.
         * 
         * @return a vector of each dice roll's values
         */
        vector<uint32_t> get_generated_sequence();

        /**
         * Sets seq to a random sequence of numbers as described in get_generated_sequence.
         * 
         * @param seq a reference to an empty vector of numbers
         */
        virtual void set_to_generated_sequence(vector<uint32_t> &seq);

        /**
         * Flips the input tile face down. If the tile is already face down,
         * this function does nothing. Otherwise, tile is subtracted from curr_score.
         * 
         * @param tile the tile to be flipped face down
         * @param curr_score the current score of the current game
         */
        void flip_tile_face_down(const uint32_t tile, uint32_t &curr_score);

        /**
         * Flips the input tile face up. If the tile is already face up,
         * this function does nothing. Otherwise, tile is added to curr_score.
         * 
         * @param tile the tile to be flipped face up
         * @param curr_score the current score of the current game
         */
        void flip_tile_face_up(const uint32_t tile, uint32_t &curr_score);

        /**
         * Simulates a game based on the strategy of the current class.
         * 
         * @param is_verbose a boolean of whether or not the simulation should print
         *  out a comprehensive log of the game
         */
        uint32_t strategy_game_simulation(Strategy *strategy, bool is_verbose=true);

        /**
         * Simulates a number of games based on the strategy of the current class.
         * Prints out the results to out.
         * 
         * @param num_games the number of games to be simulated
         * @param progress_check the number of games in which the program
         *  will let the user know how many games have been simulated so far
         * @param out the output stream of the final results
         */
        void full_strategy_simulation(Strategy *strategy, uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        /**
         * Simulates a game assuming that the player has full knowledge
         * of every roll that will happen in this game.
         * 
         * @param is_verbose a boolean of whether or not the simulation should print
         *  out a comprehensive log of the game
         */
        virtual uint32_t hindsight_game_simulation(bool is_verbose=true);

        /**
         * Recursive helper function for hindsight_game_simulation.
         * 
         * @param seq a reference to the vector as described in get_generated_sequence
         * @param idx the index of the current roll in seq
         * @param curr_score the current score of the game
         * @param is_verbose a boolean of whether or not the simulation should print
         *  out a comprehensive log of the game
         */
        uint32_t hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose=true);

        /**
         * Simulates a number of games assuming that the player has full knowledge
         * of every roll that will happen in each game.
         * 
         * @param num_games the number of games to be simulated
         * @param progress_check the number of games in which the program
         *  will let the user know how many games have been simulated so far
         * @param out the output stream of the final results
         */
        void full_hindsight_simulation(uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        /**
         * Prints the data in results to the output stream out. 
         * Varies from child to child depending on what values the dice can roll.
         * 
         * @param results the results of a probability simulation
         * @param title the display name of the simulation
         * @param out the output stream of the results
         */
        virtual void print_results(Results results, string title, uint32_t num_reached_positions, std::ostream &out=std::cout);



        Results probability_of_strategy_victory(Strategy *strategy, string csv_file_in, uint32_t progress_check=100, std::ostream &out=std::cout);
        Results probability_of_strategy_victory(Strategy *strategy, unordered_set<uint32_t> face_up_tiles_in, 
            string csv_file_in, uint32_t progress_check=100, std::ostream &out=std::cout); 

        virtual Results probability_of_strategy_victory_step(Strategy *strategy, uint32_t score_in, unordered_map<string, Results> &visited, uint32_t progress_check=100);
        virtual Results probability_of_strategy_victory_step(Strategy *strategy, std::ostream &csv_out, 
            uint32_t score_in, unordered_map<string, Results> &visited, uint32_t progress_check=100);

        Results probability_of_optimal_victory(OptimizedType sim_type=WIN_PROBABILITY, uint32_t progress_check=100, std::ostream &out=std::cout);
        Results probability_of_optimal_victory(unordered_set<uint32_t> face_up_tiles_in, OptimizedType sim_type=WIN_PROBABILITY, 
            uint32_t progress_check=100, std::ostream &out=std::cout);
        
        virtual Results probability_of_optimal_victory_step(uint32_t score_in, unordered_map<string, Results> &visited, OptimizedType sim_type=WIN_PROBABILITY, uint32_t progress_check=100);
        virtual Results probability_of_optimal_victory_step(std::ostream &csv_out, uint32_t score_in, 
            unordered_map<string, Results> &visited, OptimizedType sim_type=WIN_PROBABILITY, uint32_t progress_check=100);
};

#endif