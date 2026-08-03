#include "game.hpp"
using namespace std;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> double_dice(0,35);
std::uniform_int_distribution<std::mt19937::result_type> single_dice(1, 6);

string ComboStrategy_to_string(ComboStrategy s) {
    if (s == LARGEST_NUMBER) return "Largest Number";
    else if (s == MOST_NUMBERS) return "Most Numbers";
    else return "Unknown Strategy";
}

uint32_t roll_double() {

    int raw_probability = double_dice(rng);
    if(raw_probability == 0) return 2;
    else if (raw_probability <= 2) return 3;
    else if (raw_probability <= 5) return 4;
    else if (raw_probability <= 9) return 5;
    else if (raw_probability <= 14) return 6;
    else if (raw_probability <= 20) return 7;
    else if (raw_probability <= 25) return 8;
    else if (raw_probability <= 29) return 9;
    else if (raw_probability <= 32) return 10;
    else if (raw_probability <= 34) return 11;
    else if (raw_probability == 35) return 12;

    return 0;
}

uint32_t roll_single() {
    return single_dice(rng);
}

void Game::initialize_game(unordered_set<uint32_t> taken_numbers) {
    initial_score = 0;
    num_available = num_numbers;
    for(uint32_t i = 1; i <= num_numbers; ++i) {
        available_numbers[i] = true;
        initial_score += i;
    }

    for(auto & num : taken_numbers) {
        if(num_numbers < num || num < 1) std::cerr << "Error: Number too big for default game";
        initial_score -= num;
        available_numbers[num] = false;
        num_available--;
    }
}

Game::Game(uint32_t num_numbers_in): num_numbers(num_numbers_in) { 
    initialize_game(); 
    probabilities[1] = 1.0/6.0;
    probabilities[2] = 1.0/36.0;
    probabilities[3] = 2.0/36.0;
    probabilities[4] = 3.0/36.0;
    probabilities[5] = 4.0/36.0;
    probabilities[6] = 5.0/36.0;
    probabilities[7] = 6.0/36.0;
    probabilities[8] = 5.0/36.0;
    probabilities[9] = 4.0/36.0;
    probabilities[10] = 3.0/36.0;
    probabilities[11] = 2.0/36.0;
    probabilities[12] = 1.0/36.0;
}

vector<vector<uint32_t>> Game::get_dp(uint32_t roll_num) {
    vector<vector<uint32_t>> dp(num_numbers + 1, vector<uint32_t>(roll_num + 1, 0));
    set_dp(roll_num, dp);
    return dp;
}

void Game::set_dp(uint32_t roll_num, vector<vector<uint32_t>> &dp) {
    dp = vector<vector<uint32_t>>(num_numbers + 1, vector<uint32_t>(roll_num + 1, 0));
    for(uint32_t i = 1; i <= num_numbers; i++) {
        if (!available_numbers[i] || roll_num < i) {
            for(uint32_t j = 0; j < roll_num + 1; j++) {
                dp[i][j] = dp[i-1][j];
            }
        } else {
            for(uint32_t j = 0; j < i; j++) {
                dp[i][j] = dp[i-1][j];
            }
            dp[i][i] = dp[i-1][i] + 1;
            for(uint32_t j = i+1; j < roll_num + 1; j++) {
                dp[i][j] = dp[i-1][j] + dp[i-1][j-i];
            }
        }
    }
}

uint32_t Game::get_num_combinations(uint32_t roll_num) {
    vector<vector<uint32_t>> dp;
    set_dp(roll_num, dp);
    return dp[num_numbers][roll_num];
}

unordered_set<uint32_t> Game::get_combo_1(uint32_t roll_num) {
    unordered_set<uint32_t> combo;
    set_combo_1(roll_num, combo);
    return combo;
}
void Game::set_combo_1(uint32_t roll_num, unordered_set<uint32_t>& combo) {
    vector<vector<uint32_t>> dp;
    set_dp(roll_num, dp);

    if(dp[num_numbers][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for(uint32_t i = num_numbers; i > 0; i--) {
        if (dp[i-1][remaining_space] < dp[i][remaining_space]) {
            remaining_space -= i;
            combo.insert(i);
        }
    }
    if(remaining_space != 0) combo.clear();
}


unordered_set<uint32_t> Game::get_combo_2(uint32_t roll_num) {
    unordered_set<uint32_t> combo;
    set_combo_2(roll_num, combo);
    return combo;
}
void Game::set_combo_2(uint32_t roll_num, unordered_set<uint32_t>& combo) {
    vector<vector<uint32_t>> dp;
    set_dp(roll_num, dp);

    if(dp[num_numbers][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for(uint32_t i = num_numbers; i > 0; i--) {
        if (dp[i-1][remaining_space] == 0 && dp[i][remaining_space] > 0) {
            remaining_space -= i;
            combo.insert(i);
        }
    }
    if(remaining_space != 0) combo.clear();
}


unordered_set<uint32_t> Game::get_combo_controller(uint32_t roll_num, ComboStrategy strategy) {
    unordered_set<uint32_t> combo;
    set_combo_controller(roll_num, combo, strategy);
    return combo;
}
void Game::set_combo_controller(uint32_t roll_num, unordered_set<uint32_t>& combo, ComboStrategy strategy) {
    if (strategy == LARGEST_NUMBER) set_combo_1(roll_num, combo);
    else if (strategy == MOST_NUMBERS) set_combo_2(roll_num, combo);
}



vector<unordered_set<uint32_t>> Game::get_all_possible_combos(uint32_t roll_num) {
    vector<unordered_set<uint32_t>> combos;
    set_all_possible_combos(roll_num, combos);
    return combos;
}
void Game::set_all_possible_combos(uint32_t roll_num, vector<unordered_set<uint32_t>>& combos) {
    vector<vector<uint32_t>> dp;
    set_dp(roll_num, dp);
    if(dp[num_numbers][roll_num] == 0) return;

    queue<CombinationPath> combinations;
    
    for(uint32_t i = num_numbers; i > 0; i--) {
        if (dp[i-1][roll_num] < dp[i][roll_num]) {
            combinations.push({i-1, roll_num - i, {i}});
        }
    }

    while(combinations.size() > 0) {
        CombinationPath c = combinations.front();
        // cout << c.start_idx << " " << c.remaining_space << " | ";
        // for (auto & n : c.numbers) {
        //     cout << n << " ";
        // } cout << "\n";
        combinations.pop();
        if (c.remaining_space <= 0) {
            combos.push_back(c.numbers);
            continue;
        }
        for(uint32_t i = c.start_idx; i > 0; i--) {
            if (dp[i-1][c.remaining_space] < dp[i][c.remaining_space]) {
                c.numbers.insert(i);
                combinations.push({i-1, c.remaining_space - i, c.numbers});
                c.numbers.erase(i);
            }
        }
    }
}


vector<uint32_t> Game::get_generated_sequence() {
    vector<uint32_t> seq;
    set_generated_sequence(seq);
    return seq;
}

void Game::set_generated_sequence(vector<uint32_t>& seq) {
    uint32_t sum = 0;
    while (sum < 44) {
        uint32_t roll_num = roll_double();
        sum += roll_num;
        seq.push_back(roll_num);
    }
    if (sum == 44) {
        seq.push_back(roll_single());
    }
}




uint32_t Game::strategy_game_simulation(ComboStrategy strategy, bool is_verbose) {
    initialize_game();
    uint32_t score = initial_score;

    vector<uint32_t> seq;
    set_generated_sequence(seq);

    for(auto & roll_num : seq) {
        if(is_verbose) {
            cout << "| ";
            for(uint32_t i = 1; i <= num_numbers; i++) {
                if(available_numbers[i]) cout << i << " | ";
                else cout << "  | ";
            }
            cout << "\n";
        }

        unordered_set<uint32_t> numbers;
        set_combo_controller(roll_num, numbers, strategy);

        if(numbers.size() == 0) {
            if(is_verbose) cout << "You Lost...\n\n\n";
            return score; 
        }

        if(is_verbose) cout << "Numbers Flipped Down: ";
        for (auto & num : numbers) {
            available_numbers[num] = false;
            score -= num;
            if(is_verbose) cout << num << " ";
            num_available--;
        }
        if(is_verbose) cout << "\n\n";
    }
    if(is_verbose) cout << "You Won!!!\n";
    return score;
}

void Game::full_strategy_simulation(ComboStrategy strategy, uint32_t num_games, uint32_t progress_check, ostream &out) {
    if(num_games <= 0) return;

    cout << "\033[32mBeginning Strategy Simulation (" << ComboStrategy_to_string(strategy) << ")...\033[0m\n";

    uint32_t num_wins = 0;
    double total_score = 0;
    for(uint32_t i = 0; i < num_games; i++) {
        if(i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        uint32_t score = strategy_game_simulation(strategy, false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Strategy Simulation.\033[0m\n";

    out << "----- Strategy Simulation Results -----\n";
    out << "Strategy: " << ComboStrategy_to_string(strategy) << "\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Win Probability: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}



uint32_t Game::hindsight_game_simulation(bool is_verbose) {
    initialize_game();
    vector<uint32_t> seq;
    set_generated_sequence(seq);
    return hindsight_step(seq, 0, initial_score, is_verbose);
}

uint32_t Game::hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose) {
    if (curr_score == 0 || curr_score == seq[idx]) {
        if(is_verbose) cout << "Won!!\n";
        return 0;
    }

    if (idx >= seq.size() - 1) {
        if(is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    vector<unordered_set<uint32_t>> combos;
    set_all_possible_combos(seq[idx], combos);

    if (is_verbose) {
        cout << "Sequence Number: " << seq[idx] << "\n";
        cout << "| ";
        for(uint32_t i = 1; i <= num_numbers; i++) {
            if(available_numbers[i]) cout << i << " | ";
            else cout << "  | ";
        }
        cout << "\nCombinations:\n";
        if(combos.size() == 0) cout << "None\n";
        for(auto & combo : combos) {
            for (auto & num : combo) {
                cout << num << " ";
            }
            cout << "\n";
        }
    }

    if (combos.size() == 0) {
        if(is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    uint32_t min_score = curr_score;
    for(auto & combo : combos) {
        for (auto & num : combo) {
            curr_score -= num;
            available_numbers[num] = false;
            num_available--;
        }
        uint32_t score = hindsight_step(seq, idx+1, curr_score, is_verbose);
        if (score < min_score) min_score = score;

        if(is_verbose) cout << "Minimum Score: " << min_score << "\n";
        if(min_score == 0) return 0;
        for (auto & num : combo) {
            curr_score += num;
            available_numbers[num] = true;
            num_available++;
        }
    }
    return min_score;
}

void Game::full_hindsight_simulation(uint32_t num_games, uint32_t progress_check, ostream &out) {
    if(num_games <= 0) return;

    cout << "\033[32mBeginning Hindsight Simulation...\033[0m\n";

    uint32_t num_wins = 0;
    double total_score = 0;
    for(uint32_t i = 0; i < num_games; i++) {
        if(i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        uint32_t score = hindsight_game_simulation(false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Hindsight Simulation.\033[0m\n";

    out << "----- Hindsight Simulation Results -----\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Win Probability: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}


void Game::print_results(Results r, string title, ostream &out) {
    out << "----- " << title << " -----\n";
    out << "Average Score: " << r.avg_score << "\n";
    out << "Win Probability: " << r.win_probability * 100 << "%\n";
    out << "Best Next Decisions:\n";
    for(uint32_t i = 2; i <= 12; i++) {
        out << "Roll " << i << " --> ";
        if(r.next_combos[i].size() == 0) out << "Lose\n";
        else {
            out << "Take ";
            for (auto & num : r.next_combos[i]) {
                out << num << " ";
            }
            out << "\n";
            out << "  Average Score: " << r.next_avg_scores[i] << "\n";
            out << "  Win Probability: " << r.next_win_probabilities[i] * 100 << "%\n";
        }
    }
}



Results Game::probability_of_strategy_victory(unordered_set<uint32_t> taken_numbers, ComboStrategy strategy, std::ostream &out) {
    cout << "\033[32mBeginning Strategy Probability Simulation (" << ComboStrategy_to_string(strategy) << ")...\033[0m\n";
    initialize_game(taken_numbers);
    Results r = probability_of_strategy_victory_step(initial_score, strategy, true);
    cout << "\033[34mFinished Strategy Probability Simulation...\033[0m\n";
    print_results(r, "Strategy Probability Results (" + ComboStrategy_to_string(strategy) + ")", out);
    return r;
}

Results Game::probability_of_strategy_victory(std::ostream &csv_out, unordered_set<uint32_t> taken_numbers, ComboStrategy strategy, std::ostream &out) {
    cout << "\033[32mBeginning Strategy Probability Simulation (" << ComboStrategy_to_string(strategy) << ")...\033[0m\n";
    initialize_game(taken_numbers);
    csv_out << "Position,Win Probability,Average Score\n";
    unordered_set<string> visited;
    Results r = probability_of_strategy_victory_step(csv_out, initial_score, visited, strategy, true);
    cout << "\033[34mFinished Strategy Probability Simulation...\033[0m\n";
    print_results(r, "Strategy Probability Results (" + ComboStrategy_to_string(strategy) + ")", out);
    return r;
}

Results Game::probability_of_strategy_victory_step(uint32_t score_in, ComboStrategy strategy, bool is_first_step) {
    if (score_in == 0) return {1.0, 0};
    else if (num_available == 1) return {probabilities[score_in], (1 - probabilities[score_in]) * score_in};

    Results result;

    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if(roll_num == score_in) {
            result.win_probability += probabilities[roll_num];
            continue;
        } else if (roll_num > score_in) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        unordered_set<uint32_t> combo;
        set_combo_controller(roll_num, combo, strategy);

        if(combo.size() == 0) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }
        for (auto & num : combo) {
            curr_score -= num;
            available_numbers[num] = false;
            num_available--;
        }
        Results r = probability_of_strategy_victory_step(curr_score, strategy);

        for (auto & num : combo) {
            curr_score += num;
            available_numbers[num] = true;
            num_available++;
        }
        if (is_first_step) {
            result.next_combos[roll_num] = combo;
            result.next_avg_scores[roll_num] = r.avg_score;
            result.next_win_probabilities[roll_num] = r.win_probability;
        }   
        result.win_probability += probabilities[roll_num] * r.win_probability;
        result.avg_score += probabilities[roll_num] * r.avg_score;
    }
    return result;
}

Results Game::probability_of_strategy_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited, ComboStrategy strategy, bool is_first_step) {
    string position = "| ";
    for (uint32_t i = 1; i <= num_numbers; i++) {
        if (available_numbers[i]) position += to_string(i);
        else position += " ";
        if (i < num_numbers) position += " | ";
        else position += " |";
    }

    if (score_in == 0) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << " ,1.0,0\n";
        }
        return {1.0, 0};
    }
    else if (num_available == 1) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << position << "," << to_string(probabilities[score_in]) << "," << to_string((1 - probabilities[score_in]) * score_in) << "\n";
        }
        return {probabilities[score_in], (1 - probabilities[score_in]) * score_in};
    }

    Results result;

    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if(roll_num == score_in) {
            result.win_probability += probabilities[roll_num];
            continue;
        } else if (roll_num > score_in) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        unordered_set<uint32_t> combo;
        set_combo_controller(roll_num, combo, strategy);

        if(combo.size() == 0) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }
        for (auto & num : combo) {
            curr_score -= num;
            available_numbers[num] = false;
            num_available--;
        }
        Results r = probability_of_strategy_victory_step(csv_out, curr_score, visited, strategy);

        for (auto & num : combo) {
            curr_score += num;
            available_numbers[num] = true;
            num_available++;
        }
        if (is_first_step) {
            result.next_combos[roll_num] = combo;
            result.next_avg_scores[roll_num] = r.avg_score;
            result.next_win_probabilities[roll_num] = r.win_probability;
        }   
        result.win_probability += probabilities[roll_num] * r.win_probability;
        result.avg_score += probabilities[roll_num] * r.avg_score;
    }

    if(visited.find(position) == visited.end()) {
        visited.insert(position);
        csv_out << position << "," << result.win_probability << "," << result.avg_score << "\n";
    }

    return result;
}



Results Game::probability_of_optimal_victory(unordered_set<uint32_t> taken_numbers, std::ostream &out) {
    cout << "\033[32mBeginning Optimal Probability Simulation...\033[0m\n";
    initialize_game(taken_numbers);
    Results r = probability_of_optimal_victory_step(initial_score, true);
    print_results(r, "Optimal Probability Results", out);
    return r;

}

Results Game::probability_of_optimal_victory(std::ostream &csv_out, unordered_set<uint32_t> taken_numbers, std::ostream &out) {
    cout << "\033[32mBeginning Optimal Probability Simulation...\033[0m\n";
    initialize_game(taken_numbers);
    csv_out << "Position,Win Probability,Average Score\n";
    unordered_set<string> visited;
    Results r = probability_of_optimal_victory_step(csv_out, initial_score, visited, true);
    print_results(r, "Optimal Probability Results", out);
    return r;

}

Results Game::probability_of_optimal_victory_step(uint32_t score_in, bool is_first_step) {
    if (score_in == 0) return {1.0, 0};
    else if (num_available == 1) return {probabilities[score_in], (1 - probabilities[score_in]) * score_in};

    Results result;
    
    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if(roll_num == score_in) {
            result.win_probability += probabilities[roll_num];
            continue;
        } else if (roll_num > score_in) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> combos;
        set_all_possible_combos(roll_num, combos);

        if(combos.size() == 0) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for(auto & combo : combos) {
            for (auto & num : combo) {
                curr_score -= num;
                available_numbers[num] = false;
                num_available--;
            }
            Results r = probability_of_optimal_victory_step(curr_score);
            if(r.win_probability > max_prob) {
                max_prob = r.win_probability;
                min_score = r.avg_score;
                if(is_first_step) result.next_combos[roll_num] = combo;
            }

            for (auto & num : combo) {
                curr_score += num;
                available_numbers[num] = true;
                num_available++;
            }
        }
        if (is_first_step) {
            result.next_win_probabilities[roll_num] = max_prob;
            result.next_avg_scores[roll_num] = min_score;
        }
        result.win_probability += probabilities[roll_num] * max_prob;
        result.avg_score += probabilities[roll_num] * min_score;
    }
    return result;
}

Results Game::probability_of_optimal_victory_step(ostream &csv_out, uint32_t score_in, unordered_set<string> &visited, bool is_first_step) {
    string position = "| ";
    for (uint32_t i = 1; i <= num_numbers; i++) {
        if (available_numbers[i]) position += to_string(i);
        else position += " ";
        if (i < num_numbers) position += " | ";
        else position += " |";
    }

    if (score_in == 0) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << " ,1.0,0\n";
        }
        return {1.0, 0};
    }
    else if (num_available == 1) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << position << "," << to_string(probabilities[score_in]) << "," << to_string((1 - probabilities[score_in]) * score_in) << "\n";
        }
        return {probabilities[score_in], (1 - probabilities[score_in]) * score_in};
    }

    Results result;
    
    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if(roll_num == score_in) {
            result.win_probability += probabilities[roll_num];
            continue;
        } else if (roll_num > score_in) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> combos;
        set_all_possible_combos(roll_num, combos);

        if(combos.size() == 0) {
            result.avg_score += probabilities[roll_num] * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for(auto & combo : combos) {
            for (auto & num : combo) {
                curr_score -= num;
                available_numbers[num] = false;
                num_available--;
            }
            Results r = probability_of_optimal_victory_step(csv_out, curr_score, visited);
            if(r.win_probability > max_prob) {
                max_prob = r.win_probability;
                min_score = r.avg_score;
                if(is_first_step) result.next_combos[roll_num] = combo;
            }

            for (auto & num : combo) {
                curr_score += num;
                available_numbers[num] = true;
                num_available++;
            }
        }
        if (is_first_step) {
            result.next_win_probabilities[roll_num] = max_prob;
            result.next_avg_scores[roll_num] = min_score;
        }
        result.win_probability += probabilities[roll_num] * max_prob;
        result.avg_score += probabilities[roll_num] * min_score;
    }

    if(visited.find(position) == visited.end()) {
        visited.insert(position);
        csv_out << position << "," << result.win_probability << "," << result.avg_score << "\n";
    }

    return result;
}



int main() {
    Game g;
    ofstream hindsight_out("hindsight_results.txt");
    ofstream largest_number_out("largest_number_results.txt");
    ofstream largest_number_csv_out("largest_number.csv");
    ofstream optimal_csv_out("optimal_probabilities.csv");
    // g.full_strategy_simulation(ComboStrategy::LARGEST_NUMBER, 10000, 10000, largest_number_out);
    // g.full_hindsight_simulation(10000, 10000, hindsight_out);
    g.probability_of_strategy_victory(largest_number_csv_out);
    //g.probability_of_optimal_victory(optimal_csv_out);
}