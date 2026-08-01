#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
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
    int start_idx;
    int remaining_space;
    unordered_set<int> numbers;
};

string ComboStrategy_to_string(ComboStrategy s);
int roll_double();
int roll_single();

class Game {
    private:
        int num_available;
        int initial_score;
        unordered_set<int> playable_numbers;
        unordered_map<int, bool> available_numbers;
        virtual void initialize_game();
        virtual void initialize_game_position(unordered_set<int> taken_numbers);
    public:
        Game();

        vector<vector<int>> get_dp(int roll_num);
        int get_num_combinations(int roll_num);
        unordered_set<int> get_combo_1(int roll_num);
        unordered_set<int> get_combo_2(int roll_num);
        vector<unordered_set<int>> get_all_possible_combos(int roll_num);
        vector<int> generate_sequence();


        int strategy_game_simulation(ComboStrategy strategy=LARGEST_NUMBER, bool is_verbose=true);
        void full_strategy_simulation(std::ostream &out=std::cout, ComboStrategy strategy=LARGEST_NUMBER, int num_games=100000, int progress_check=10000);


        int optimal_strategy_game_simulation(bool is_verbose=true);
        int optimal_step(vector<int> &seq, size_t idx, int curr_score, bool is_verbose=true);
        void full_optimal_simulation(std::ostream &out=std::cout, int num_games=100000, int progress_check=10000);
};

#endif