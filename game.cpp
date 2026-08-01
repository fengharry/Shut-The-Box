#include "game.hpp"
using namespace std;

std::random_device dev;
std::mt19937 rng(dev());

string ComboStrategy_to_string(ComboStrategy s) {
    if (s == LARGEST_NUMBER) return "Largest Number";
    else if (s == MOST_NUMBERS) return "Most Numbers";
    else return "Unknown Strategy";
}

int roll_double() {
    std::uniform_int_distribution<std::mt19937::result_type> double_dice(0,35);

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

    return -1;
}

int roll_single() {
    std::uniform_int_distribution<std::mt19937::result_type> single_dice(1, 6);
    return single_dice(rng);
}

void Game::initialize_game() {
    initial_score = 45;
    num_available = 9;
    for(int i = 1; i <= 9; ++i) {
        available_numbers[i] = true;
    }
}

void Game::initialize_game_position(unordered_set<int> taken_numbers) {
    initialize_game();
    for(auto & num : taken_numbers) {
        if(9 < num || num < 1) std::cerr << "Error: Number too big for default game";
        initial_score -= num;
        available_numbers[num] = false;
        num_available--;
    }
}

Game::Game() { initialize_game(); }

vector<vector<int>> Game::get_dp(int roll_num) {
    vector<vector<int>> dp(10, vector<int>(roll_num + 1, 0));
    for(int i = 1; i < 10; i++) {
        if (!available_numbers[i] || roll_num < i) {
            for(int j = 0; j < roll_num + 1; j++) {
                dp[i][j] = dp[i-1][j];
            }
        } else {
            for(int j = 0; j < i; j++) {
                dp[i][j] = dp[i-1][j];
            }
            dp[i][i] = dp[i-1][i] + 1;
            for(int j = i+1; j < roll_num + 1; j++) {
                dp[i][j] = dp[i-1][j] + dp[i-1][j-i];
            }
        }
    }
    return dp;
}

int Game::get_num_combinations(int roll_num) {
    vector<vector<int>> dp = get_dp(roll_num);
    return dp[9][roll_num];
}

unordered_set<int> Game::get_combo_1(int roll_num) {
    vector<vector<int>> dp = get_dp(roll_num);

    unordered_set<int> result;
    if(dp[9][roll_num] == 0) return result;
    
    int remaining_space = roll_num;
    for(int i = 9; i > 0; i--) {
        if (dp[i-1][remaining_space] < dp[i][remaining_space]) {
            remaining_space -= i;
            result.insert(i);
        }
    }
    if(remaining_space != 0) return unordered_set<int>();
    return result;
}

unordered_set<int> Game::get_combo_2(int roll_num) {
    vector<vector<int>> dp = get_dp(roll_num);

    unordered_set<int> result;
    if(dp[9][roll_num] == 0) return result;
    
    int remaining_space = roll_num;
    for(int i = 9; i > 0; i--) {
        if (dp[i-1][remaining_space] == 0 && dp[i][remaining_space] > 0) {
            remaining_space -= i;
            result.insert(i);
        }
    }
    if(remaining_space != 0) return unordered_set<int>();
    return result;
}

vector<unordered_set<int>> Game::get_all_possible_combos(int roll_num) {
    vector<vector<int>> dp = get_dp(roll_num);
    vector<unordered_set<int>> result;
    if(dp[9][roll_num] == 0) return {};

    queue<CombinationPath> combinations;
    
    for(int i = 9; i > 0; i--) {
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
            result.push_back(c.numbers);
            continue;
        }
        for(int i = c.start_idx; i > 0; i--) {
            if (dp[i-1][c.remaining_space] < dp[i][c.remaining_space]) {
                c.numbers.insert(i);
                combinations.push({i-1, c.remaining_space - i, c.numbers});
                c.numbers.erase(i);
            }
        }
    }
    return result;
}

vector<int> Game::generate_sequence() {
    vector<int> sequence;
    int sum = 0;
    while (sum < 44) {
        int roll_num = roll_double();
        sum += roll_num;
        sequence.push_back(roll_num);
    }
    if (sum == 44) {
        sequence.push_back(roll_single());
    }
    return sequence;
}



int Game::strategy_game_simulation(ComboStrategy strategy, bool is_verbose) {
    initialize_game();
    int score = initial_score;

    vector<int> seq = generate_sequence();
    for(auto & roll_num : seq) {
        if(is_verbose) {
            cout << "| ";
            for(int i = 1; i < 10; i++) {
                if(available_numbers[i]) cout << i << " | ";
                else cout << "  | ";
            }
            cout << "\n";
        }

        unordered_set<int> numbers;
        if(strategy == LARGEST_NUMBER) numbers = get_combo_1(roll_num);
        else if(strategy == MOST_NUMBERS) numbers = get_combo_2(roll_num);
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

void Game::full_strategy_simulation(ostream &out, ComboStrategy strategy, int num_games, int progress_check) {
    if(num_games <= 0) return;

    cout << "\033[32mBeginning Strategy Simulation (" << ComboStrategy_to_string(strategy) << ")...\033[0m\n";

    int num_wins = 0;
    double total_score = 0;
    for(int i = 0; i < num_games; i++) {
        if(i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        int score = strategy_game_simulation(strategy, false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Strategy Simulation.\033[0m\n";

    out << "----- Strategy Simulation Results -----\n";
    out << "Strategy: " << ComboStrategy_to_string(strategy) << "\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Odds of Victory: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}



int Game::optimal_strategy_game_simulation(bool is_verbose) {
    initialize_game();
    vector<int> seq = generate_sequence();
    return optimal_step(seq, 0, initial_score, is_verbose);
}

int Game::optimal_step(vector<int> &seq, size_t idx, int curr_score, bool is_verbose) {
    if (curr_score <= 0 || curr_score == seq[idx]) {
        if(is_verbose) cout << "Won!!\n";
        return 0;
    }
    if (idx == seq.size() - 1) {
        if(is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    vector<unordered_set<int>> combos = get_all_possible_combos(seq[idx]);

    if (is_verbose) {
        cout << "Sequence Number: " << seq[idx] << "\n";
        cout << "| ";
        for(int i = 1; i < 10; i++) {
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

    int min_score = min(45, curr_score);
    for(auto & combo : combos) {
        for (auto & num : combo) {
            curr_score -= num;
            available_numbers[num] = false;
            num_available--;
        }
        int score = optimal_step(seq, idx+1, curr_score, is_verbose);
        if (score < min_score) min_score = score;

        if(is_verbose) cout << "Minimum Score: " << min_score << "\n";
        if(min_score == 0) return min_score;
        for (auto & num : combo) {
            curr_score += num;
            available_numbers[num] = true;
            num_available++;
        }
    }
    return min_score;
}

void Game::full_optimal_simulation(ostream &out, int num_games, int progress_check) {
    if(num_games <= 0) return;

    cout << "\033[32mBeginning Optimal Simulation...\033[0m\n";

    int num_wins = 0;
    double total_score = 0;
    for(int i = 0; i < num_games; i++) {
        if(i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        int score = optimal_strategy_game_simulation(false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Optimal Simulation.\033[0m\n";

    out << "----- Optimal Simulation Results -----\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Odds of Victory: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}


int main() {
    Game g;
    ofstream optimal_out("optimal_results.txt");
    ofstream largest_number_out("largest_number_results.txt");
    g.full_strategy_simulation(largest_number_out, ComboStrategy::LARGEST_NUMBER, 1000000, 100000);
    g.full_optimal_simulation(optimal_out, 1000000, 100000);
}