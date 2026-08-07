#include "ShutTheBox.hpp"
#include <algorithm>
using namespace std;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> default_double_dice(0,35);
std::uniform_int_distribution<std::mt19937::result_type> default_single_dice(1, 6);

uint32_t ShutTheBox::roll_double(bool use_default) {
    if(use_default) {
        int raw_probability = default_double_dice(rng);
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

    uint32_t size = 0;
    for (const auto &probability : dice_probabilities[2]) {
        if (probability.first > size) size = probability.first;
    }
    vector<double> probs(size, 0);
    for (const auto &probability : dice_probabilities[2]) {
        probs[probability.first] = probability.second;
    }
    std::discrete_distribution<std::mt19937::result_type> double_dice(probs.begin(), probs.end());
    return double_dice(rng);
    
}

uint32_t ShutTheBox::roll_single(bool use_default) {
    if(use_default) return default_single_dice(rng);

    uint32_t size = 0;
    for (const auto &probability : dice_probabilities[1]) {
        if (probability.first > size) size = probability.first;
    }
    vector<double> probs(size, 0);
    for (const auto &probability : dice_probabilities[1]) {
        probs[probability.first] = probability.second;
    }
    std::discrete_distribution<std::mt19937::result_type> single_dice(probs.begin(), probs.end());
    return single_dice(rng);
}

void ShutTheBox::get_all_positions(uint32_t tile_idx, string curr_position) {
    if (tile_idx >= sorted_tiles.size()) {
        all_positions.insert(curr_position);
        return;
    }

    uint32_t &tile = sorted_tiles[tile_idx];
    get_all_positions(tile_idx+1, curr_position + " " + to_string(tile) + " |");
    get_all_positions(tile_idx+1, curr_position + "   |");
}

void ShutTheBox::csv_record_unreachable_positions(std::ostream &csv_out, unordered_set<string> &reached_positions) const {
    for(string position : all_positions) {
        if (reached_positions.find(position) == reached_positions.end()) {
            csv_out << position << ",N/A,N/A";
            for(uint32_t roll_num = 1; roll_num <= 12; roll_num++) {
                csv_out << ",N/A";
            }
            csv_out << "\n";
        }
    }
}

void ShutTheBox::csv_record_position(std::ostream &csv_out, string curr_position, Results &results) const {
    csv_out << curr_position << "," << results.win_probability << "," << results.avg_score << ",";
    for(uint32_t roll_num = 1; roll_num <= 12; roll_num++) {
        auto it = results.next_tile_combinations.find(roll_num);
        if (it == results.next_tile_combinations.end()) csv_out << "N/A";
        else {
            uint32_t curr_sum = 0;
            for (const uint32_t &tile : it->second) {
                curr_sum += tile;
                if (curr_sum >= roll_num) csv_out << tile;
                else csv_out << tile << " ";
            }
        }
        if (roll_num < 12) csv_out << ",";
    }
    csv_out << "\n";
}



void ShutTheBox::initialize_game() {
    initial_score = 0;
    num_face_up = 0;
    for(uint32_t tile : tiles) {
        flip_tile_face_up(tile, initial_score);
    }
}

void ShutTheBox::initialize_game(unordered_set<uint32_t> face_up_tiles_in) {
    initial_score = 0;
    num_face_up = 0;

    for(uint32_t tile : tiles) {
        // cout << tile << " ";
        is_tile_face_up[tile] = false;
    }

    for(uint32_t tile : face_up_tiles_in) {
        if(tiles.find(tile) == tiles.end()) std::cerr << "Error: Invalid Tile";
        flip_tile_face_up(tile, initial_score);
    }
}

ShutTheBox::ShutTheBox(uint32_t num_tiles_in): strategy_name("Largest Number") { 
    for(uint32_t tile = 1; tile <= num_tiles_in; tile++) {
        tiles.insert(tile);
        sorted_tiles.push_back(tile);
        empty_position += "   |";
        if (tile > largest_tile) largest_tile = tile;
    }
    sort(sorted_tiles.begin(), sorted_tiles.end());

    get_all_positions();
    initialize_game(); 

    dice_probabilities[1][1] = 1.0/6.0;
    dice_probabilities[1][2] = 1.0/6.0;
    dice_probabilities[1][3] = 1.0/6.0;
    dice_probabilities[1][4] = 1.0/6.0;
    dice_probabilities[1][5] = 1.0/6.0;
    dice_probabilities[1][6] = 1.0/6.0;

    dice_probabilities[2][2] = 1.0/36.0;
    dice_probabilities[2][3] = 2.0/36.0;
    dice_probabilities[2][4] = 3.0/36.0;
    dice_probabilities[2][5] = 4.0/36.0;
    dice_probabilities[2][6] = 5.0/36.0;
    dice_probabilities[2][7] = 6.0/36.0;
    dice_probabilities[2][8] = 5.0/36.0;
    dice_probabilities[2][9] = 4.0/36.0;
    dice_probabilities[2][10] = 3.0/36.0;
    dice_probabilities[2][11] = 2.0/36.0;
    dice_probabilities[2][12] = 1.0/36.0;
}

ShutTheBox::ShutTheBox(unordered_set<uint32_t> tiles_in): 
    strategy_name("Largest Number"), tiles(tiles_in) { 
    for(const uint32_t tile : tiles) {
        sorted_tiles.push_back(tile);
        empty_position += "   |";
        if (tile > largest_tile) largest_tile = tile;
    }
    sort(sorted_tiles.begin(), sorted_tiles.end());

    get_all_positions();
    initialize_game(); 
}

ShutTheBox::ShutTheBox(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, unordered_map<uint32_t, double>> dice_probabilities_in): 
    ShutTheBox(tiles_in) { 
    dice_probabilities = dice_probabilities_in;
}



string ShutTheBox::get_curr_position() {
    string position;
    set_curr_position(position);
    return position;
}
void ShutTheBox::set_curr_position(string &position) {
    position = "|";
    for (uint32_t tile : sorted_tiles) {
        if (is_tile_face_up[tile]) position += " " + to_string(tile) + " |";
        else position += "   |";
    }
}



vector<vector<uint32_t>> ShutTheBox::get_tile_combinations_dp(const uint32_t roll_num) {
    vector<vector<uint32_t>> tile_combinations_dp(largest_tile + 1, vector<uint32_t>(roll_num + 1, 0));
    set_tile_combinations_dp(roll_num, tile_combinations_dp);
    return tile_combinations_dp;
}

void ShutTheBox::set_tile_combinations_dp(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp) {
    tile_combinations_dp = vector<vector<uint32_t>>(largest_tile + 1, vector<uint32_t>(roll_num + 1, 0));
    for(uint32_t i = 1; i <= largest_tile; i++) {
        if (!is_tile_face_up[i] || roll_num < i) {
            for(uint32_t j = 0; j < roll_num + 1; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j];
            }
        } else {
            for(uint32_t j = 0; j < i; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j];
            }
            tile_combinations_dp[i][i] = tile_combinations_dp[i-1][i] + 1;
            for(uint32_t j = i+1; j < roll_num + 1; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j] + tile_combinations_dp[i-1][j-i];
            }
        }
    }
}

uint32_t ShutTheBox::get_num_combinations(const uint32_t roll_num) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_tile_combinations_dp(roll_num, tile_combinations_dp);
    return tile_combinations_dp[tiles.size()][roll_num];
}

unordered_set<uint32_t> ShutTheBox::get_combination(const uint32_t roll_num) {
    unordered_set<uint32_t> tile_combination;
    set_combination(roll_num, tile_combination);
    return tile_combination;
}
void ShutTheBox::set_combination(const uint32_t roll_num, unordered_set<uint32_t>& tile_combination) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_tile_combinations_dp(roll_num, tile_combinations_dp);

    if(tile_combinations_dp[tiles.size()][roll_num] == 0) return;
    
    uint32_t remaining_space = roll_num;
    for(uint32_t i = tiles.size(); i > 0; i--) {
        if (tile_combinations_dp[i-1][remaining_space] < tile_combinations_dp[i][remaining_space]) {
            remaining_space -= i;
            tile_combination.insert(i);
        }
    }
    if(remaining_space != 0) tile_combination.clear();
}


// unordered_set<uint32_t> ShutTheBox::get_combination_2(const uint32_t roll_num) {
//     unordered_set<uint32_t> tile_combination;
//     set_combination_2(roll_num, tile_combination);
//     return tile_combination;
// }
// void ShutTheBox::set_combination(const uint32_t roll_num, unordered_set<uint32_t>& tile_combination) {
//     vector<vector<uint32_t>> tile_combinations_dp;
//     set_tile_combinations_dp(roll_num, tile_combinations_dp);

//     if(tile_combinations_dp[tiles.size()][roll_num] == 0) return;
    
//     uint32_t remaining_space = roll_num;
//     for(uint32_t i = tiles.size(); i > 0; i--) {
//         if (tile_combinations_dp[i-1][remaining_space] == 0 && tile_combinations_dp[i][remaining_space] > 0) {
//             remaining_space -= i;
//             tile_combination.insert(i);
//         }
//     }
//     if(remaining_space != 0) tile_combination.clear();
// }



vector<unordered_set<uint32_t>> ShutTheBox::get_all_possible_tile_combinations(const uint32_t roll_num) {
    vector<unordered_set<uint32_t>> tile_combinations;
    set_all_possible_tile_combinations(roll_num, tile_combinations);
    return tile_combinations;
}
void ShutTheBox::set_all_possible_tile_combinations(const uint32_t roll_num, vector<unordered_set<uint32_t>>& tile_combinations) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_tile_combinations_dp(roll_num, tile_combinations_dp);
    if(tile_combinations_dp[tiles.size()][roll_num] == 0) return;

    queue<CombinationPath> combinations_queue;
    
    for(uint32_t i = tiles.size(); i > 0; i--) {
        if (tile_combinations_dp[i-1][roll_num] < tile_combinations_dp[i][roll_num]) {
            combinations_queue.push({i-1, roll_num - i, {i}});
        }
    }

    while(combinations_queue.size() > 0) {
        CombinationPath c = combinations_queue.front();
        combinations_queue.pop();
        if (c.remaining_space <= 0) {
            tile_combinations.push_back(c.numbers);
            continue;
        }
        for(uint32_t i = c.start_idx; i > 0; i--) {
            if (tile_combinations_dp[i-1][c.remaining_space] < tile_combinations_dp[i][c.remaining_space]) {
                c.numbers.insert(i);
                combinations_queue.push({i-1, c.remaining_space - i, c.numbers});
                c.numbers.erase(i);
            }
        }
    }
}


vector<uint32_t> ShutTheBox::get_generated_sequence() {
    vector<uint32_t> seq;
    set_generated_sequence(seq);
    return seq;
}

void ShutTheBox::set_generated_sequence(vector<uint32_t>& seq) {
    uint32_t roll_sum = 0;
    while (roll_sum < 44) {
        uint32_t roll_num = roll_double();
        roll_sum += roll_num;
        seq.push_back(roll_num);
    }
    if (roll_sum == 44) {
        seq.push_back(roll_single());
    }
}



void ShutTheBox::flip_tile_face_down(const uint32_t tile, uint32_t &curr_score) {
    if (!is_tile_face_up[tile]) return;
    is_tile_face_up[tile] = false;
    curr_score -= tile;
    num_face_up--;
}

void ShutTheBox::flip_tile_face_up(const uint32_t tile, uint32_t &curr_score) {
    if (is_tile_face_up[tile]) return;
    is_tile_face_up[tile] = true;
    curr_score += tile;
    num_face_up++;
}



uint32_t ShutTheBox::strategy_game_simulation(bool is_verbose) {
    initialize_game();
    uint32_t score = initial_score;

    vector<uint32_t> seq;
    set_generated_sequence(seq);

    for(const uint32_t & roll_num : seq) {
        if(is_verbose) {
            string position;
            set_curr_position(position);
            cout << position << "\n";
        }

        unordered_set<uint32_t> tile_combination;
        set_combination(roll_num, tile_combination);

        if(tile_combination.size() == 0) {
            if(is_verbose) cout << "You Lost...\n\n\n";
            return score; 
        }

        if(is_verbose) cout << "Numbers Flipped Down: ";
        for (const uint32_t &tile : tile_combination) {
            if(is_verbose) cout << tile << " ";
            flip_tile_face_down(tile, score);
        }
        if(is_verbose) cout << "\n\n";
    }
    if(is_verbose) cout << "You Won!!!\n";
    return score;
}

void ShutTheBox::full_strategy_simulation(uint32_t num_games, uint32_t progress_check, ostream &out) {
    if(num_games <= 0) return;

    cout << "\033[32mBeginning Strategy Simulation (" << strategy_name << ")...\033[0m\n";

    uint32_t num_wins = 0;
    double total_score = 0;
    for(uint32_t i = 0; i < num_games; i++) {
        if(i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        uint32_t score = strategy_game_simulation(false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Strategy Simulation.\033[0m\n";

    out << "----- Strategy Simulation Results -----\n";
    out << "Strategy: " << strategy_name << "\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Win Probability: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}



uint32_t ShutTheBox::hindsight_game_simulation(bool is_verbose) {
    initialize_game();
    vector<uint32_t> seq;
    set_generated_sequence(seq);
    return hindsight_step(seq, 0, initial_score, is_verbose);
}

uint32_t ShutTheBox::hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose) {
    if (curr_score == 0 || curr_score == seq[idx]) {
        if(is_verbose) cout << "Won!!\n";
        return 0;
    }

    if (idx >= seq.size() - 1) {
        if(is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    vector<unordered_set<uint32_t>> tile_combinations;
    set_all_possible_tile_combinations(seq[idx], tile_combinations);

    if (is_verbose) {
        cout << "Sequence Number: " << seq[idx] << "\n";
        
        string position;
        set_curr_position(position);
        cout << position << "\n";
        
        cout << "Combinations:\n";
        if(tile_combinations.size() == 0) cout << "None\n";
        for(const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t & tile : tile_combination) {
                cout << tile << " ";
            }
            cout << "\n";
        }
    }

    if (tile_combinations.size() == 0) {
        if(is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    uint32_t min_score = curr_score;
    for(const unordered_set<uint32_t> &tile_combination : tile_combinations) {
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        uint32_t score = hindsight_step(seq, idx+1, curr_score, is_verbose);
        if (score < min_score) min_score = score;

        if(is_verbose) cout << "Minimum Score: " << min_score << "\n";
        if(min_score == 0) return 0;
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
    }
    return min_score;
}

void ShutTheBox::full_hindsight_simulation(uint32_t num_games, uint32_t progress_check, ostream &out) {
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


void ShutTheBox::print_results(Results results, string title, ostream &out) const {
    out << "----- " << title << " -----\n";
    out << "Average Score: " << results.avg_score << "\n";
    out << "Win Probability: " << results.win_probability * 100 << "%\n";
    out << "Best Next Decisions:\n";
    for(uint32_t i = 2; i <= 12; i++) {
        out << "Roll " << i << " --> ";
        if(results.next_tile_combinations[i].size() == 0) out << "Lose\n";
        else {
            out << "Take ";
            for (const uint32_t &tile : results.next_tile_combinations[i]) {
                out << tile << " ";
            }
            out << "\n";
            out << "  Average Score: " << results.next_avg_scores[i] << "\n";
            out << "  Win Probability: " << results.next_win_probabilities[i] * 100 << "%\n";
        }
    }
}

Results ShutTheBox::probability_of_strategy_victory() {
    return probability_of_strategy_victory(tiles, std::cout);
}

Results ShutTheBox::probability_of_strategy_victory(std::ostream &out, bool is_output_csv) {
    if(is_output_csv) return probability_of_strategy_victory(out, tiles);
    else return probability_of_strategy_victory(tiles, out);
}

Results ShutTheBox::probability_of_strategy_victory(std::ostream &csv_out, std::ostream &out) {
    return probability_of_strategy_victory(csv_out, tiles, out);
}

Results ShutTheBox::probability_of_strategy_victory(unordered_set<uint32_t> face_up_tiles_in, std::ostream &out) {
    cout << "\033[32mBeginning Strategy Probability Simulation (" << strategy_name << ")...\033[0m\n";
    initialize_game(face_up_tiles_in);
    Results results = probability_of_strategy_victory_step(initial_score);
    cout << "\033[34mFinished Strategy Probability Simulation...\033[0m\n";
    print_results(results, "Strategy Probability Results (" + strategy_name + ")", out);
    return results;
}

Results ShutTheBox::probability_of_strategy_victory(std::ostream &csv_out, unordered_set<uint32_t> face_up_tiles_in, std::ostream &out) {
    cout << "\033[32mBeginning Strategy Probability Simulation (" << strategy_name << ")...\033[0m\n";
    initialize_game(face_up_tiles_in);

    csv_out << "Position,Win Probability,Average Score,1,2,3,4,5,6,7,8,9,10,11,12\n";
    unordered_set<string> visited;

    visited.insert(empty_position);
    csv_out << empty_position << ",1.0,0";
    for(uint32_t roll_num = 1; roll_num <= 12; roll_num++) {
        csv_out << ",N/A";
    }
    csv_out << "\n";

    Results results = probability_of_strategy_victory_step(csv_out, initial_score, visited);
    
    csv_record_unreachable_positions(csv_out, visited);

    cout << "\033[34mFinished Strategy Probability Simulation...\033[0m\n";
    print_results(results, "Strategy Probability Results (" + strategy_name + ")", out);
    return results;
}

Results ShutTheBox::probability_of_strategy_victory_step(uint32_t score_in) {
    if (score_in == 0) return {1.0, 0};
    
    Results results;
    if (num_face_up == 1) {
        if(score_in == 1) {
            results.win_probability = dice_probabilities[1][score_in];
            results.avg_score = (1 - dice_probabilities[1][score_in]) * score_in;
        } else {
            results.win_probability = dice_probabilities[2][score_in];
            results.avg_score = (1 - dice_probabilities[2][score_in]) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        return results;
    }


    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if(roll_num == score_in) {
            results.win_probability += dice_probabilities[2][roll_num];
            continue;
        } else if (roll_num > score_in) {
            results.avg_score += dice_probabilities[2][roll_num] * score_in;
            continue;
        }

        unordered_set<uint32_t> tile_combination;
        set_combination(roll_num, tile_combination);

        if(tile_combination.size() == 0) {
            results.avg_score += dice_probabilities[2][roll_num] * score_in;
            continue;
        }
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        Results roll_results = probability_of_strategy_victory_step(curr_score);

        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
        results.next_tile_combinations[roll_num] = tile_combination;
        results.next_avg_scores[roll_num] = roll_results.avg_score;
        results.next_win_probabilities[roll_num] = roll_results.win_probability;
        results.win_probability += dice_probabilities[2][roll_num] * roll_results.win_probability;
        results.avg_score += dice_probabilities[2][roll_num] * roll_results.avg_score;
    }
    return results;
}

Results ShutTheBox::probability_of_strategy_victory_step(std::ostream &csv_out, uint32_t score_in, unordered_set<string> &visited) {

    string position;
    set_curr_position(position);

    Results results;

    if (score_in == 0 || num_face_up == 0) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << " ,1.0,0";
            for(uint32_t roll_num = 1; roll_num <= 12; roll_num++) {
                csv_out << ",N/A";
            }
            csv_out << "\n";
        }
        
        return {1.0, 0};
    }
    if (num_face_up == 1) {
        if(score_in == 1) {
            results.win_probability = dice_probabilities[1][score_in];
            results.avg_score = (1 - dice_probabilities[1][score_in]) * score_in;
        } else {
            results.win_probability = dice_probabilities[2][score_in];
            results.avg_score = (1 - dice_probabilities[2][score_in]) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_record_position(csv_out, position, results);
        }
        return results;
    }

    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        double &roll_probability = dice_probabilities[2][roll_num];
        if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        unordered_set<uint32_t> tile_combination;
        set_combination(roll_num, tile_combination);

        if(tile_combination.size() == 0) {
            // cout << position << "\n";
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t curr_score = score_in;
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        // cout << curr_score << " " << num_face_up << "\n";
        Results roll_results = probability_of_strategy_victory_step(csv_out, curr_score, visited);

        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
        results.next_tile_combinations[roll_num] = tile_combination;
        results.next_avg_scores[roll_num] = roll_results.avg_score;
        results.next_win_probabilities[roll_num] = roll_results.win_probability;  
        results.win_probability += roll_probability * roll_results.win_probability;
        results.avg_score += roll_probability * roll_results.avg_score;
    }

    if(visited.find(position) == visited.end()) {
        visited.insert(position);
        csv_record_position(csv_out, position, results);
    }

    return results;
}





Results ShutTheBox::probability_of_optimal_victory() {
    return probability_of_optimal_victory(tiles);
}

Results ShutTheBox::probability_of_optimal_victory(std::ostream &out, bool is_output_csv) {
    if (is_output_csv) return probability_of_optimal_victory(out, tiles);
    else return probability_of_optimal_victory(tiles, out);
}

Results ShutTheBox::probability_of_optimal_victory(std::ostream &csv_out, std::ostream &out) {
    return probability_of_optimal_victory(csv_out, tiles, out);
}

Results ShutTheBox::probability_of_optimal_victory(unordered_set<uint32_t> face_up_tiles_in, std::ostream &out) {
    cout << "\033[32mBeginning Optimal Probability Simulation...\033[0m\n";
    initialize_game(face_up_tiles_in);
    Results results = probability_of_optimal_victory_step(initial_score);
    print_results(results, "Optimal Probability Results", out);
    return results;
}

Results ShutTheBox::probability_of_optimal_victory(std::ostream &csv_out, unordered_set<uint32_t> face_up_tiles_in, std::ostream &out) {
    cout << "\033[32mBeginning Optimal Probability Simulation...\033[0m\n";
    initialize_game(face_up_tiles_in);
    csv_out << "Position,Win Probability,Average Score,1,2,3,4,5,6,7,8,9,10,11,12\n";
    unordered_set<string> visited;

    visited.insert(empty_position);
    csv_out << empty_position << ",1.0,0";
    for(uint32_t roll_num = 1; roll_num <= 12; roll_num++) {
        csv_out << ",N/A";
    }
    csv_out << "\n";

    Results results = probability_of_optimal_victory_step(csv_out, initial_score, visited);

    csv_record_unreachable_positions(csv_out, visited);

    print_results(results, "Optimal Probability Results", out);
    return results;
}

Results ShutTheBox::probability_of_optimal_victory_step(uint32_t score_in) {
    if (score_in == 0) return {1.0, 0};

    Results results;
    if (num_face_up == 1) {
        if(score_in == 1) {
            results.win_probability = dice_probabilities[1][score_in];
            results.avg_score = (1 - dice_probabilities[1][score_in]) * score_in;
        } else {
            results.win_probability = dice_probabilities[2][score_in];
            results.avg_score = (1 - dice_probabilities[2][score_in]) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};
        
        return results;
    }
    
    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        double &roll_probability = dice_probabilities[2][roll_num];
        if(roll_num == score_in) {
            results.win_probability += roll_probability;
            continue;
        } else if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> tile_combinations;
        set_all_possible_tile_combinations(roll_num, tile_combinations);

        if(tile_combinations.size() == 0) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for(const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_down(tile, curr_score);
            }
            Results roll_results = probability_of_optimal_victory_step(curr_score);
            if(roll_results.win_probability > max_prob) {
                max_prob = roll_results.win_probability;
                min_score = roll_results.avg_score;
                results.next_tile_combinations[roll_num] = tile_combination;
            }

            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_up(tile, curr_score);
            }
        }
        results.next_win_probabilities[roll_num] = max_prob;
        results.next_avg_scores[roll_num] = min_score;
        results.win_probability += roll_probability * max_prob;
        results.avg_score += roll_probability * min_score;
    }
    return results;
}

Results ShutTheBox::probability_of_optimal_victory_step(ostream &csv_out, uint32_t score_in, unordered_set<string> &visited) {
    
    string position;
    set_curr_position(position);

    if (score_in == 0) {
        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_out << " ,1.0,0\n";
        }
        return {1.0, 0};
    }

    Results results;
    
    if (num_face_up == 1) {
        if(score_in == 1) {
            results.win_probability = dice_probabilities[1][score_in];
            results.avg_score = (1 - dice_probabilities[1][score_in]) * score_in;
        } else {
            results.win_probability = dice_probabilities[2][score_in];
            results.avg_score = (1 - dice_probabilities[2][score_in]) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if(visited.find(position) == visited.end()) {
            visited.insert(position);
            csv_record_position(csv_out, position, results);
        }

        return results;
    }
    
    uint32_t curr_score = score_in;
    for(uint32_t roll_num = 2; roll_num <= 12; roll_num++) {
        if (roll_num > score_in) {
            results.avg_score += dice_probabilities[2][roll_num] * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> tile_combinations;
        set_all_possible_tile_combinations(roll_num, tile_combinations);

        if(tile_combinations.size() == 0) {
            results.avg_score += dice_probabilities[2][roll_num] * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for(const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_down(tile, curr_score);
            }
            Results roll_results = probability_of_optimal_victory_step(csv_out, curr_score, visited);
            if(roll_results.win_probability > max_prob) {
                max_prob = roll_results.win_probability;
                min_score = roll_results.avg_score;
                results.next_tile_combinations[roll_num] = tile_combination;
            }

            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_up(tile, curr_score);
            }
        }
        results.next_win_probabilities[roll_num] = max_prob;
        results.next_avg_scores[roll_num] = min_score;
        results.win_probability += dice_probabilities[2][roll_num] * max_prob;
        results.avg_score += dice_probabilities[2][roll_num] * min_score;
    }

    if(visited.find(position) == visited.end()) {
        visited.insert(position);
        csv_record_position(csv_out, position, results);
    }

    return results;
}



int main() {
    ShutTheBox g;
    // g.initialize_game({1, 9});
    // ofstream hindsight_out("hindsight_results.txt");
    // ofstream largest_number_out("largest_number_results.txt");
    ofstream largest_number_csv_out("largest_number.csv");
    //ofstream optimal_csv_out("optimal_probabilities.csv");
    // g.full_strategy_simulation(10000, 10000, largest_number_out);
    // g.full_hindsight_simulation(10000, 10000, hindsight_out);
    g.probability_of_strategy_victory(largest_number_csv_out, true);
    //g.probability_of_optimal_victory(optimal_csv_out);
    // unordered_set<uint32_t> r = g.get_combination(10);
    // for (uint32_t t : r) {
    //     cout << t << " ";
    // }
}