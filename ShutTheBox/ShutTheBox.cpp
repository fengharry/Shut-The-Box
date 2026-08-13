#include "ShutTheBox.hpp"
#include <algorithm>
using namespace std;


uint32_t ShutTheBox::roll_double() {
    return dice.roll(2);
}

uint32_t ShutTheBox::roll_single() {
    return dice.roll(1);
}

void ShutTheBox::get_all_positions(uint32_t tile_idx, string curr_position) {
    if (tile_idx >= sorted_tiles.size()) {
        all_positions.insert(curr_position);
        return;
    }

    uint32_t &tile = sorted_tiles[tile_idx];
    if (curr_position != "") get_all_positions(tile_idx+1, curr_position + " " + to_string(tile));
    else get_all_positions(tile_idx+1, curr_position + to_string(tile));
    get_all_positions(tile_idx+1, curr_position);
}

void ShutTheBox::csv_record_unreachable_positions(std::ostream &csv_out, unordered_map<string, Results> &reached_positions) {
    for (string position : all_positions) {
        if (reached_positions.find(position) == reached_positions.end()) {
            csv_out << position << ",N/A,N/A";
            
            vector<uint32_t> possible_rolls;
            dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
            for (uint32_t roll_num : possible_rolls) {
                csv_out << ",N/A";
            }
            csv_out << "\n";
        }
    }
}

void ShutTheBox::csv_record_position(std::ostream &csv_out, string curr_position, Results &results) {
    csv_out << curr_position << "," << results.win_probability << "," << results.avg_score << ",";
    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
    for (uint32_t roll_idx = 0; roll_idx < possible_rolls.size(); roll_idx++) {
        uint32_t &roll_num = possible_rolls[roll_idx];
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
        if (roll_idx < possible_rolls.size() - 1) csv_out << ",";
    }
    csv_out << "\n";
}



void ShutTheBox::initialize_game() {
    initial_score = 0;
    num_face_up = 0;
    for (uint32_t tile : tiles) {
        flip_tile_face_up(tile, initial_score);
    }
}

void ShutTheBox::initialize_game(unordered_set<uint32_t> face_up_tiles_in) {
    initial_score = 0;
    num_face_up = 0;

    for (uint32_t tile : tiles) {
        // cout << tile << " ";
        is_tile_face_up[tile] = false;
    }

    for (uint32_t tile : face_up_tiles_in) {
        if (tiles.find(tile) == tiles.end()) std::cerr << "Error: Invalid Tile";
        flip_tile_face_up(tile, initial_score);
    }
}


ShutTheBox::ShutTheBox(uint32_t num_tiles_in, string optimal_win_csv_file_in, string optimal_score_csv_file_in) { 
    if (optimal_win_csv_file_in == "") optimal_win_csv_file = "optimal_win_" + to_string(num_tiles_in) + ".csv";
    else optimal_win_csv_file = optimal_win_csv_file_in;

    if (optimal_score_csv_file == "") optimal_score_csv_file = "optimal_score_" + to_string(num_tiles_in) + ".csv";
    else optimal_score_csv_file = optimal_score_csv_file_in;

    for (uint32_t tile = 1; tile <= num_tiles_in; tile++) {
        tiles.insert(tile);
        sorted_tiles.push_back(tile);

        if (tile > largest_tile) largest_tile = tile;
        tile_sum += tile;
    }
    sort(sorted_tiles.begin(), sorted_tiles.end());

    get_all_positions();
    initialize_game(); 
}

ShutTheBox::ShutTheBox(unordered_set<uint32_t> tiles_in, string optimal_win_csv_file_in, string optimal_score_csv_file_in): tiles(tiles_in) { 
    if (optimal_win_csv_file_in == "") optimal_win_csv_file = "optimal_win_" + to_string(tiles_in.size()) + ".csv";
    else optimal_win_csv_file = optimal_win_csv_file_in;

    if (optimal_score_csv_file == "") optimal_score_csv_file = "optimal_score_" + to_string(tiles_in.size()) + ".csv";
    else optimal_score_csv_file = optimal_score_csv_file_in;

    for (const uint32_t tile : tiles) {
        sorted_tiles.push_back(tile);

        if (tile > largest_tile) largest_tile = tile;
        tile_sum += tile;
    }
    sort(sorted_tiles.begin(), sorted_tiles.end());

    get_all_positions();
    initialize_game(); 
}

ShutTheBox::ShutTheBox(unordered_set<uint32_t> tiles_in, unordered_map<uint32_t, double> single_die_probabilities_in, 
string optimal_win_csv_file_in, string optimal_score_csv_file_in): ShutTheBox(tiles_in, optimal_win_csv_file, optimal_score_csv_file) { 
    dice = Dice(single_die_probabilities_in);
}



string ShutTheBox::get_curr_position() {
    string position;
    set_to_curr_position(position);
    return position;
}
void ShutTheBox::set_to_curr_position(string &position) {
    position = "";
    for (uint32_t tile_idx = 0; tile_idx < sorted_tiles.size(); tile_idx++) {
        uint32_t &tile = sorted_tiles[tile_idx];

        if (is_tile_face_up[tile]) {
            if (position == "") position += to_string(tile);
            else position += " " + to_string(tile);
        }
    }
}



vector<vector<uint32_t>> ShutTheBox::get_tile_combinations_dp(const uint32_t roll_num) {
    vector<vector<uint32_t>> tile_combinations_dp(largest_tile + 1, vector<uint32_t>(roll_num + 1, 0));
    set_to_tile_combinations_dp(roll_num, tile_combinations_dp);
    return tile_combinations_dp;
}

void ShutTheBox::set_to_tile_combinations_dp(const uint32_t roll_num, vector<vector<uint32_t>> &tile_combinations_dp) {
    tile_combinations_dp = vector<vector<uint32_t>>(largest_tile + 1, vector<uint32_t>(roll_num + 1, 0));
    for (uint32_t i = 1; i <= largest_tile; i++) {
        if (!is_tile_face_up[i] || roll_num < i) {
            for (uint32_t j = 0; j < roll_num + 1; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j];
            }
        } else {
            for (uint32_t j = 0; j < i; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j];
            }
            tile_combinations_dp[i][i] = tile_combinations_dp[i-1][i] + 1;
            for (uint32_t j = i+1; j < roll_num + 1; j++) {
                tile_combinations_dp[i][j] = tile_combinations_dp[i-1][j] + tile_combinations_dp[i-1][j-i];
            }
        }
    }
}

uint32_t ShutTheBox::get_num_combinations(const uint32_t roll_num) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_to_tile_combinations_dp(roll_num, tile_combinations_dp);
    return tile_combinations_dp[largest_tile][roll_num];
}

unordered_set<uint32_t> ShutTheBox::get_strategy_combination(Strategy *strategy, const uint32_t roll_num) {
    unordered_set<uint32_t> tile_combination;
    set_to_strategy_combination(strategy, roll_num, tile_combination);
    return tile_combination;
}
void ShutTheBox::set_to_strategy_combination(Strategy *strategy, const uint32_t roll_num, unordered_set<uint32_t> &tile_combination) {
    vector<vector<uint32_t>> tile_combinations_dp;
    set_to_tile_combinations_dp(roll_num, tile_combinations_dp);
    strategy->set_to_combination(roll_num, tile_combination, tile_combinations_dp);
}



vector<uint32_t> ShutTheBox::get_generated_sequence() {
    vector<uint32_t> seq;
    set_to_generated_sequence(seq);
    return seq;
}

void ShutTheBox::set_to_generated_sequence(vector<uint32_t> &seq) {
    seq.clear();
    uint32_t roll_sum = 0;
    while (roll_sum < tile_sum - 1) {
        uint32_t roll_num = roll_double();
        roll_sum += roll_num;
        seq.push_back(roll_num);
    }
    if (roll_sum == tile_sum - 1) {
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



uint32_t ShutTheBox::strategy_game_simulation(Strategy *strategy, bool is_verbose) {
    initialize_game();
    uint32_t score = initial_score;

    vector<uint32_t> seq;
    set_to_generated_sequence(seq);

    for (const uint32_t & roll_num : seq) {
        if (is_verbose) {
            string position;
            set_to_curr_position(position);
            cout << position << "\n";
        }

        unordered_set<uint32_t> tile_combination;
        set_to_strategy_combination(strategy, roll_num, tile_combination);

        if (tile_combination.size() == 0) {
            if (is_verbose) cout << "You Lost...\n\n\n";
            return score; 
        }

        if (is_verbose) cout << "Numbers Flipped Down: ";
        for (const uint32_t &tile : tile_combination) {
            if (is_verbose) cout << tile << " ";
            flip_tile_face_down(tile, score);
        }
        if (is_verbose) cout << "\n\n";
    }
    if (is_verbose) cout << "You Won!!!\n";
    return score;
}

void ShutTheBox::full_strategy_simulation(Strategy *strategy, uint32_t num_games, uint32_t progress_check, ostream &out) {
    if (num_games <= 0) return;

    cout << "\033[32mBeginning Strategy Simulation (" << strategy->get_name() << ")...\033[0m\n";

    uint32_t num_wins = 0;
    double total_score = 0;
    for (uint32_t i = 0; i < num_games; i++) {
        if (i % progress_check == 0) cout << " Simulated " << i << " Games\n";
        uint32_t score = strategy_game_simulation(strategy, false);
        num_wins += score == 0 ? 1 : 0;
        total_score += score;
    }
    cout << "\033[34mFinished Strategy Simulation.\033[0m\n";

    out << "----- Strategy Simulation Results -----\n";
    out << "Strategy: " << strategy->get_name() << "\n";
    out << "Number of Games Simulated: " << num_games << "\n";
    out << "Number of Wins: " << num_wins << "\n";
    out << "Win Probability: " << double(num_wins) / double(num_games) * 100 << "%\n";
    out << "Average Score: " << total_score / double(num_games) << "\n";
}



uint32_t ShutTheBox::hindsight_game_simulation(bool is_verbose) {
    initialize_game();
    vector<uint32_t> seq;
    set_to_generated_sequence(seq);
    return hindsight_step(seq, 0, initial_score, is_verbose);
}

uint32_t ShutTheBox::hindsight_step(vector<uint32_t> &seq, size_t idx, uint32_t curr_score, bool is_verbose) {
    if (curr_score == 0 || curr_score == seq[idx]) {
        if (is_verbose) cout << "Won!!\n";
        return 0;
    }

    if (idx >= seq.size() - 1) {
        if (is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    uint32_t &roll_num = seq[idx];

    vector<unordered_set<uint32_t>> tile_combinations;
    vector<vector<uint32_t>> tile_combinations_dp;
    set_to_tile_combinations_dp(roll_num, tile_combinations_dp);

    set_to_all_possible_tile_combinations(roll_num, tile_combinations, tile_combinations_dp);

    if (is_verbose) {
        cout << "Rolled Number: " << roll_num << "\n";
        
        string position;
        set_to_curr_position(position);
        cout << position << "\n";
        
        cout << "Combinations:\n";
        if (tile_combinations.size() == 0) cout << "None\n";
        for (const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t & tile : tile_combination) {
                cout << tile << " ";
            }
            cout << "\n";
        }
    }

    if (tile_combinations.size() == 0) {
        if (is_verbose) cout << "Lost, Score: " << curr_score << "\n";
        return curr_score;
    }

    uint32_t min_score = curr_score;
    for (const unordered_set<uint32_t> &tile_combination : tile_combinations) {
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        uint32_t score = hindsight_step(seq, idx+1, curr_score, is_verbose);
        if (score < min_score) min_score = score;

        if (is_verbose) cout << "Minimum Score: " << min_score << "\n";
        if (min_score == 0) return 0;
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
    }
    return min_score;
}

void ShutTheBox::full_hindsight_simulation(uint32_t num_games, uint32_t progress_check, ostream &out) {
    if (num_games <= 0) return;

    cout << "\033[32mBeginning Hindsight Simulation...\033[0m\n";

    uint32_t num_wins = 0;
    double total_score = 0;
    for (uint32_t i = 0; i < num_games; i++) {
        if (i % progress_check == 0) cout << " Simulated " << i << " Games\n";
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


void ShutTheBox::print_results(Results results, string title, uint32_t num_reached_positions, ostream &out) {
    out << "----- " << title << " -----\n";
    out << "Number of Reached Positions: " << num_reached_positions << "\n";
    out << "Number of Unreached Positions: " << all_positions.size() - num_reached_positions << "\n";
    out << "Average Score: " << results.avg_score << "\n";
    out << "Win Probability: " << results.win_probability * 100 << "%\n";
    out << "Best Next Decisions:\n";

    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
    for (uint32_t roll_num : possible_rolls) {
        if (roll_num == 1 && results.next_tile_combinations[1].size() == 0) continue;
        
        out << "Roll " << roll_num << " --> ";
        if (results.next_tile_combinations[roll_num].size() == 0) out << "Lose\n";
        else {
            out << "Take ";
            for (const uint32_t &tile : results.next_tile_combinations[roll_num]) {
                out << tile << " ";
            }
            out << "\n";
            out << "  Average Score: " << results.next_avg_scores[roll_num] << "\n";
            out << "  Win Probability: " << results.next_win_probabilities[roll_num] * 100 << "%\n";
        }
    }
}

Results ShutTheBox::probability_of_strategy_victory(Strategy *strategy, string csv_file_in, uint32_t progress_check, std::ostream &out) {
    return probability_of_strategy_victory(strategy, tiles, csv_file_in, progress_check, out);
}

Results ShutTheBox::probability_of_strategy_victory(Strategy *strategy, unordered_set<uint32_t> face_up_tiles_in, 
string csv_file_in, uint32_t progress_check, std::ostream &out) {
    std::ofstream csv_out("../results/" + csv_file_in);
    cout << "\033[32mBeginning Strategy Probability Simulation (" << strategy->get_name() << ")...\033[0m\n";
    initialize_game(face_up_tiles_in);

    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);

    csv_out << "Position,Win Probability,Average Score";
     for (uint32_t roll_num : possible_rolls) {
        csv_out << "," << roll_num;
    }
    csv_out << "\n";

    unordered_map<string, Results> visited;
    visited[""] = empty_results;

    csv_out << ",1.0,0";
    for (uint32_t roll_num : possible_rolls) {
        csv_out << ",N/A";
    }
    csv_out << "\n";

    Results results = probability_of_strategy_victory_step(strategy, csv_out, initial_score, visited, progress_check);
    
    csv_record_unreachable_positions(csv_out, visited);

    cout << "\033[34mFinished Strategy Probability Simulation.\033[0m\n";
    print_results(results, "Strategy Probability Results (" + strategy->get_name() + ")", visited.size(), out);
    return results;
}

Results ShutTheBox::probability_of_strategy_victory_step(Strategy *strategy, uint32_t score_in, unordered_map<string, Results> &visited, uint32_t progress_check) {
    string position;
    set_to_curr_position(position);

    Results results;

    if (score_in == 0 || num_face_up == 0) {
        if (visited.find(position) == visited.end()) {
            visited[""] = empty_results;
        }
        
        return {1.0, 0};
    }
    if (num_face_up == 1) {
        if (score_in == 1) {
            results.win_probability = dice.get_probability(score_in, num_dice_min);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_min)) * score_in;
        } else {
            results.win_probability = dice.get_probability(score_in, num_dice_max);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_max)) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if (visited.find(position) == visited.end()) {
            visited[position] = results;
            if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
        }
        return results;
    }

    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_max, num_dice_max, possible_rolls);
    for (uint32_t roll_num : possible_rolls) {
        double roll_probability = dice.get_probability(roll_num, num_dice_max);
        if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        unordered_set<uint32_t> tile_combination;
        set_to_strategy_combination(strategy, roll_num, tile_combination);

        if (tile_combination.size() == 0) {
            // cout << position << "\n";
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t curr_score = score_in;
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        // cout << curr_score << " " << num_face_up << "\n";
        string curr_position = get_curr_position();
        Results roll_results;
        if (visited.find(curr_position) != visited.end()) roll_results = visited[curr_position];
        else roll_results = probability_of_strategy_victory_step(strategy, curr_score, visited, progress_check);

        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
        results.next_tile_combinations[roll_num] = tile_combination;
        results.next_avg_scores[roll_num] = roll_results.avg_score;
        results.next_win_probabilities[roll_num] = roll_results.win_probability;  
        results.win_probability += roll_probability * roll_results.win_probability;
        results.avg_score += roll_probability * roll_results.avg_score;
    }

    if (visited.find(position) == visited.end()) {
        visited[position] = results;
        if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
    }

    return results;
}

Results ShutTheBox::probability_of_strategy_victory_step(Strategy *strategy, std::ostream &csv_out, uint32_t score_in, unordered_map<string, Results> &visited, uint32_t progress_check) {
    string position;
    set_to_curr_position(position);

    Results results;

    if (score_in == 0 || num_face_up == 0) {
        if (visited.find(position) == visited.end()) {
            visited[""] = empty_results;

            csv_out << ",1.0,0";
            vector<uint32_t> possible_rolls;
            dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
            for (uint32_t roll_num : possible_rolls) {
                csv_out << ",N/A";
            }
            csv_out << "\n";
        }
        
        return {1.0, 0};
    }
    if (num_face_up == 1) {
        if (score_in == 1) {
            results.win_probability = dice.get_probability(score_in, num_dice_min);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_min)) * score_in;
        } else {
            results.win_probability = dice.get_probability(score_in, num_dice_max);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_max)) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if (visited.find(position) == visited.end()) {
            visited[position] = results;
            csv_record_position(csv_out, position, results);
            if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
        }
        return results;
    }

    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_max, num_dice_max, possible_rolls);
    for (uint32_t roll_num : possible_rolls) {
        double roll_probability = dice.get_probability(roll_num, num_dice_max);
        if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        unordered_set<uint32_t> tile_combination;
        set_to_strategy_combination(strategy, roll_num, tile_combination);

        if (tile_combination.size() == 0) {
            // cout << position << "\n";
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t curr_score = score_in;
        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_down(tile, curr_score);
        }
        // cout << curr_score << " " << num_face_up << "\n";
        string curr_position = get_curr_position();
        Results roll_results;
        if (visited.find(curr_position) != visited.end()) roll_results = visited[curr_position];
        else roll_results = probability_of_strategy_victory_step(strategy, csv_out, curr_score, visited, progress_check);

        for (const uint32_t &tile : tile_combination) {
            flip_tile_face_up(tile, curr_score);
        }
        results.next_tile_combinations[roll_num] = tile_combination;
        results.next_avg_scores[roll_num] = roll_results.avg_score;
        results.next_win_probabilities[roll_num] = roll_results.win_probability;  
        results.win_probability += roll_probability * roll_results.win_probability;
        results.avg_score += roll_probability * roll_results.avg_score;
    }

    if (visited.find(position) == visited.end()) {
        visited[position] = results;
        csv_record_position(csv_out, position, results);
        if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
    }

    return results;
}



Results ShutTheBox::probability_of_optimal_victory(OptimizedType sim_type, uint32_t progress_check, std::ostream &out) {
    return probability_of_optimal_victory(tiles, sim_type, progress_check, out);
}

Results ShutTheBox::probability_of_optimal_victory(unordered_set<uint32_t> face_up_tiles_in, OptimizedType sim_type, uint32_t progress_check, std::ostream &out) {
    string csv_file = "";
    string title = "Optimal Probability Results ";
    string optimized_type_title = "";

    if (sim_type == WIN_PROBABILITY) {
        csv_file = "../results/" + optimal_win_csv_file;
        optimized_type_title = "(Win Probability)";
    }
    else if (sim_type == AVERAGE_SCORE) {
        csv_file = "../results/" + optimal_score_csv_file;
        optimized_type_title = "(Average Score)";
    }
    std::ofstream csv_out(csv_file);

    cout << "\033[32mBeginning Optimal Probability Simulation " + optimized_type_title + "...\033[0m\n";
    initialize_game(face_up_tiles_in);
    
    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);

    csv_out << "Position,Win Probability,Average Score";
     for (uint32_t roll_num : possible_rolls) {
        csv_out << "," << roll_num;
    }
    csv_out << "\n";

    unordered_map<string, Results> visited;
    visited[""] = empty_results;

    csv_out << ",1.0,0";
    for (uint32_t roll_num : possible_rolls) {
        csv_out << ",N/A";
    }
    csv_out << "\n";

    Results results = probability_of_optimal_victory_step(csv_out, initial_score, visited, sim_type, progress_check);

    csv_record_unreachable_positions(csv_out, visited);

    cout << "\033[34mFinished Optimal Simulation.\033[0m\n";
    print_results(results, title + optimized_type_title, visited.size(), out);

    return results;
}

Results ShutTheBox::probability_of_optimal_victory_step(uint32_t score_in, unordered_map<string, Results> &visited, OptimizedType sim_type, uint32_t progress_check) {

    string position;
    set_to_curr_position(position);

    if (score_in == 0 || num_face_up == 0) {
        if (visited.find(position) == visited.end()) {
            visited[""] = empty_results;
        }
        return {1.0, 0};
    }

    Results results;
    
    if (num_face_up == 1) {
        if (score_in == 1) {
            results.win_probability = dice.get_probability(score_in, num_dice_min);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_min)) * score_in;
        } else {
            results.win_probability = dice.get_probability(score_in, num_dice_max);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_max)) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if (visited.find(position) == visited.end()) {
            visited[position] = results;
            if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
        }

        return results;
    }
    
    uint32_t curr_score = score_in;
    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_max, num_dice_max, possible_rolls);
    for (uint32_t roll_num : possible_rolls) {
        double roll_probability = dice.get_probability(roll_num, num_dice_max);
        if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> tile_combinations;
        vector<vector<uint32_t>> tile_combinations_dp;
        set_to_tile_combinations_dp(roll_num, tile_combinations_dp);

        set_to_all_possible_tile_combinations(roll_num, tile_combinations, tile_combinations_dp);

        if (tile_combinations.size() == 0) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for (const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_down(tile, curr_score);
            }
            string curr_position = get_curr_position();
            Results roll_results;
            if (visited.find(curr_position) != visited.end()) roll_results = visited[curr_position];
            else roll_results = probability_of_optimal_victory_step(curr_score, visited, sim_type, progress_check);

            if ((sim_type == WIN_PROBABILITY && roll_results.win_probability > max_prob) ||
                (sim_type == AVERAGE_SCORE && roll_results.avg_score < min_score)) {
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

    if (visited.find(position) == visited.end()) {
        visited[position] = results;
        if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
    }

    return results;
}

Results ShutTheBox::probability_of_optimal_victory_step(ostream &csv_out, uint32_t score_in, 
unordered_map<string, Results> &visited, OptimizedType sim_type, uint32_t progress_check) {
    
    string position;
    set_to_curr_position(position);

    if (score_in == 0 || num_face_up == 0) {
        if (visited.find(position) == visited.end()) {
            visited[""] = empty_results;
            
            csv_out << " ,1.0,0";
            vector<uint32_t> possible_rolls;
            dice.set_to_possible_rolls(num_dice_min, num_dice_max, possible_rolls);
            for (uint32_t roll_num : possible_rolls) {
                csv_out << ",N/A";
            }
            csv_out << "\n";
        }
        return {1.0, 0};
    }

    Results results;
    
    if (num_face_up == 1) {
        if (score_in == 1) {
            results.win_probability = dice.get_probability(score_in, num_dice_min);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_min)) * score_in;
        } else {
            results.win_probability = dice.get_probability(score_in, num_dice_max);
            results.avg_score = (1 - dice.get_probability(score_in, num_dice_max)) * score_in;
        }
        results.next_tile_combinations[score_in] = {score_in};

        if (visited.find(position) == visited.end()) {
            visited[position] = results;
            csv_record_position(csv_out, position, results);
            if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
        }

        return results;
    }
    
    uint32_t curr_score = score_in;
    vector<uint32_t> possible_rolls;
    dice.set_to_possible_rolls(num_dice_max, num_dice_max, possible_rolls);
    for (uint32_t roll_num : possible_rolls) {
        double roll_probability = dice.get_probability(roll_num, num_dice_max);
        if (roll_num > score_in) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        vector<unordered_set<uint32_t>> tile_combinations;
        vector<vector<uint32_t>> tile_combinations_dp;
        set_to_tile_combinations_dp(roll_num, tile_combinations_dp);

        set_to_all_possible_tile_combinations(roll_num, tile_combinations, tile_combinations_dp);

        if (tile_combinations.size() == 0) {
            results.avg_score += roll_probability * score_in;
            continue;
        }

        uint32_t min_score = score_in;
        double max_prob = 0;
        for (const unordered_set<uint32_t> &tile_combination : tile_combinations) {
            for (const uint32_t &tile : tile_combination) {
                flip_tile_face_down(tile, curr_score);
            }
            string curr_position = get_curr_position();
            Results roll_results;
            if (visited.find(curr_position) != visited.end()) roll_results = visited[curr_position];
            else roll_results = probability_of_optimal_victory_step(csv_out, curr_score, visited, sim_type, progress_check);

            if ((sim_type == WIN_PROBABILITY && roll_results.win_probability > max_prob) ||
                (sim_type == AVERAGE_SCORE && roll_results.avg_score < min_score)) {
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

    if (visited.find(position) == visited.end()) {
        visited[position] = results;
        csv_record_position(csv_out, position, results);
        if (visited.size() % progress_check == 0) cout << "Explored " << visited.size() << " Unique Combinations\n";
    }

    return results;
}