#ifndef SHUT_THE_BOX_HPP
#define SHUT_THE_BOX_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <random>
using namespace std;

struct CombinationPath {
    uint32_t start_idx;
    uint32_t remaining_space;
    unordered_set<uint32_t> numbers;
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
        string strategy_name = "";
        
        string empty_position = "|";
        unordered_set<string> all_positions;
        vector<uint32_t> sorted_tiles;
        uint32_t largest_tile = 0;

        unordered_set<uint32_t> tiles;
        unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities;

        uint32_t initial_score = 0;
        uint32_t num_face_up = 0;
        unordered_map<uint32_t, bool> is_tile_face_up;

        uint32_t roll_double(const bool use_default=true);
        uint32_t roll_single(const bool use_default=true);

        void get_all_positions(uint32_t tile_idx=0, string curr_position="|");

        void csv_record_unreachable_positions(std::ostream &csv_out, unordered_set<string> &reached_positions) const;
        void csv_record_position(std::ostream &csv_out, string curr_position, Results &results) const;

    public:
        virtual void initialize_game();
        virtual void initialize_game(unordered_set<uint32_t> face_up_tiles_in);

        ShutTheBox(uint32_t num_tiles_in=9);
        ShutTheBox(unordered_set<uint32_t> tiles_in);
        ShutTheBox(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities_in);

        string get_curr_position();
        void set_curr_position(string &position);

        vector<vector<uint32_t>> get_tile_combinations_dp(const uint32_t roll_num);
        void set_tile_combinations_dp(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp);

        uint32_t get_num_combinations(const uint32_t roll_num);

        virtual unordered_set<uint32_t> get_combination(const uint32_t roll_num);
        virtual void set_combination(const uint32_t roll_num, unordered_set<uint32_t>& combo);

        vector<unordered_set<uint32_t>> get_all_possible_tile_combinations(const uint32_t roll_num);
        void set_all_possible_tile_combinations(const uint32_t roll_num, vector<unordered_set<uint32_t>>& combos);

        vector<uint32_t> get_generated_sequence();
        void set_generated_sequence(vector<uint32_t>& seq);

        void flip_tile_face_down(const uint32_t tile, uint32_t &curr_score);
        void flip_tile_face_up(const uint32_t tile, uint32_t &curr_score);

        uint32_t strategy_game_simulation(bool is_verbose=true);
        void full_strategy_simulation(uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        uint32_t hindsight_game_simulation(bool is_verbose=true);
        uint32_t hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose=true);
        void full_hindsight_simulation(uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        void print_results(Results results, string title, std::ostream &out=std::cout) const;



        Results probability_of_strategy_victory();
        Results probability_of_strategy_victory(std::ostream &out, bool is_output_csv);
        Results probability_of_strategy_victory(std::ostream &csv_out, std::ostream &out=std::cout);
        Results probability_of_strategy_victory(unordered_set<uint32_t> face_up_tiles_in, std::ostream &out=std::cout);
        Results probability_of_strategy_victory(std::ostream &csv_out, unordered_set<uint32_t> face_up_tiles_in, std::ostream &out=std::cout);
        
        Results probability_of_strategy_victory_step(uint32_t score_in=false);
        Results probability_of_strategy_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited);

        Results probability_of_optimal_victory();
        Results probability_of_optimal_victory(std::ostream &out, bool is_output_csv);
        Results probability_of_optimal_victory(std::ostream &csv_out, std::ostream &out=std::cout);
        Results probability_of_optimal_victory(unordered_set<uint32_t> face_up_tiles_in, std::ostream &out=std::cout);
        Results probability_of_optimal_victory(std::ostream &csv_out, unordered_set<uint32_t> face_up_tiles_in, std::ostream &out=std::cout);

        Results probability_of_optimal_victory_step(uint32_t score_in=false);
        Results probability_of_optimal_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited);
};

#endif