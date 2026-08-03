#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <random>
using namespace std;

enum ComboStrategy {
    LARGEST_NUMBER, MOST_NUMBERS
};

struct CombinationPath {
    uint32_t start_idx;
    uint32_t remaining_space;
    unordered_set<uint32_t> numbers;
};

struct Results {
    double win_probability = 0;
    double avg_score = 0;
    unordered_map<uint32_t, unordered_set<uint32_t>> next_combos;
    unordered_map<uint32_t, double> next_win_probabilities;
    unordered_map<uint32_t, double> next_avg_scores;
};

string ComboStrategy_to_string(ComboStrategy s);
uint32_t roll_double();
uint32_t roll_single();

class Game {
    private:
        uint32_t num_numbers;
        unordered_map<uint32_t, double> probabilities;
        
        uint32_t num_available;
        uint32_t initial_score;
        unordered_map<uint32_t, bool> available_numbers;
        virtual void initialize_game(unordered_set<uint32_t> taken_numbers={});
    public:
        Game(uint32_t num_numbers_in=9);

        vector<vector<uint32_t>> get_dp(uint32_t roll_num);
        void set_dp(uint32_t roll_num, vector<vector<uint32_t>> &dp);

        uint32_t get_num_combinations(uint32_t roll_num);

        unordered_set<uint32_t> get_combo_1(uint32_t roll_num);
        void set_combo_1(uint32_t roll_num, unordered_set<uint32_t>& combo);

        unordered_set<uint32_t> get_combo_2(uint32_t roll_num);
        void set_combo_2(uint32_t roll_num, unordered_set<uint32_t>& combo);

        unordered_set<uint32_t> get_combo_controller(uint32_t roll_num, ComboStrategy strategy=LARGEST_NUMBER);
        void set_combo_controller(uint32_t roll_num, unordered_set<uint32_t>& combo, ComboStrategy strategy=LARGEST_NUMBER);

        vector<unordered_set<uint32_t>> get_all_possible_combos(uint32_t roll_num);
        void set_all_possible_combos(uint32_t roll_num, vector<unordered_set<uint32_t>>& combos);

        vector<uint32_t> get_generated_sequence();
        void set_generated_sequence(vector<uint32_t>& seq);



        uint32_t strategy_game_simulation(ComboStrategy strategy=LARGEST_NUMBER, bool is_verbose=true);
        void full_strategy_simulation(ComboStrategy strategy=LARGEST_NUMBER, uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        uint32_t hindsight_game_simulation(bool is_verbose=true);
        uint32_t hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose=true);
        void full_hindsight_simulation(uint32_t num_games=100000, uint32_t progress_check=10000, std::ostream &out=std::cout);

        void print_results(Results r, string title, std::ostream &out=std::cout);

        Results probability_of_strategy_victory(unordered_set<uint32_t> taken_numbers={}, ComboStrategy strategy=LARGEST_NUMBER, std::ostream &out=std::cout);
        Results probability_of_strategy_victory(std::ostream &csv_out, unordered_set<uint32_t> taken_numbers={}, ComboStrategy strategy=LARGEST_NUMBER, std::ostream &out=std::cout);
        
        Results probability_of_strategy_victory_step(uint32_t score_in, ComboStrategy strategy=LARGEST_NUMBER, bool is_first_step=false);
        Results probability_of_strategy_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited, ComboStrategy strategy=LARGEST_NUMBER, bool is_first_step=false);


        Results probability_of_optimal_victory(unordered_set<uint32_t> taken_numbers={}, std::ostream &out=std::cout);
        Results probability_of_optimal_victory(std::ostream &csv_out, unordered_set<uint32_t> taken_numbers={}, std::ostream &out=std::cout);

        Results probability_of_optimal_victory_step(uint32_t score_in, bool is_first_step=false);
        Results probability_of_optimal_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited, bool is_first_step=false);
};

#endif