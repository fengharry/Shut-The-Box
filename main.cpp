#include "ShutTheBox/ShutTheBox.hpp"
#include "Strategies/StrategyList.hpp"
#include <getopt.h>
#include <cctype>    
#include <algorithm>
using namespace std;

bool is_char_equal(char a, char b) {
    return tolower(static_cast<unsigned char>(a)) ==
           tolower(static_cast<unsigned char>(b));
}
bool is_str_equal(const string &a, const string &b) {
    return equal(a.begin(), a.end(), b.begin(), b.end(), is_char_equal);
}


const vector<string> strategies = {"LargestNumber, MostNumbers, SaveOne"};

Strategy* get_custom_strategy(const string &strategy_name) {
    if (is_str_equal(strategy_name, "LargestNumber")) return new LargestNumber();
    else if (is_str_equal(strategy_name, "MostNumbers")) return new MostNumbers();
    else if (is_str_equal(strategy_name, "SaveOne")) return new SaveOne();
    else return nullptr;
}

bool is_valid_strategy(const string &strategy_name, const string &run_type) {
    if ((is_str_equal(strategy_name, "OptimalWinProbability") || 
        is_str_equal(strategy_name, "OptimalAverageScore")) &&
        is_str_equal(run_type, "CALCULATION")) return true;
    else if (is_str_equal(strategy_name, "Hindsight") &&
            is_str_equal(run_type, "SIMULATION")) return true;
    
    Strategy *strategy = get_custom_strategy(strategy_name);
    if (strategy != nullptr) {
        delete strategy;
        return true;
    } 

    return false;
}

int main(int argc, char** argv) {
    int c;
    uint32_t game_size = 0;
    string run_type = "", strategy_name = "", optimal_setting = "", sim_file = "", calc_csv_file = "";
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            { "game_size",     required_argument, 0, 'g'  },
            { "run_type",      required_argument, 0, 'r'  },
            { "strategy",      required_argument, 0, 's'  },
            { "sim_file",      required_argument, 0, 'f' },
            { "calc_csv_file", required_argument, 0, 'c' },
        };

        c = getopt_long(argc, argv, "g:r:s:f:c:",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'g':
            game_size = stoi(optarg);
            if (game_size == 0) {
                cerr << "ERROR: game_size cannot be 0.\n";
                return 1;
            }
            break;

        case 'r':
            run_type = optarg;
            if (!is_str_equal(run_type, "SIMULATION") && !is_str_equal(run_type, "CALCULATION")) {
                cerr << "ERROR: Invalid run_type.\n";
                return 1;
            }
            break;

        case 's':
            strategy_name = optarg;
            break;

        case 'f':
            sim_file = optarg;
            break;

        case 'c':
            calc_csv_file = optarg;
            if(!is_str_equal(calc_csv_file.substr(calc_csv_file.size() - 4), ".csv")) calc_csv_file += ".csv";
            break;

        default:
            cout << "ERROR: Unknown command line option.";
            break;
        }
    }

    if (run_type == "") {
        cerr << "ERROR: run_type was not provided.\n";
        return 1;
    } else if (game_size == 0) {
        cerr << "ERROR: game_size was not provided.\n";
        return 1;
    } else if (strategy_name == "") {
        cerr << "ERROR: strategy_name was not provided.\n";
        return 1;
    } else if (!is_valid_strategy(strategy_name, run_type)) {
        cerr << "ERROR: Strategy '" << strategy_name << "' is invalid.\n";
        return 1;
    }

    ShutTheBox g(game_size);

    if (run_type == "SIMULATION") {    
        std::ostream* results_out = &std::cout;
        if (sim_file != "") {
            std::ofstream file_out("../results/" + sim_file);
            results_out = &file_out;
        }

        if (is_str_equal(strategy_name, "Hindsight")) g.full_hindsight_simulation(*results_out);
        else {
            Strategy* strategy = get_custom_strategy(strategy_name);
            g.full_strategy_simulation(strategy, *results_out);
            delete strategy;
        }
    }
    else if (run_type == "CALCULATION") {
        if (calc_csv_file == "") {
            cerr << "ERROR: calc_csv_file was not provided.\n";
            return 1;
        }

        if (is_str_equal(strategy_name, "OptimalWinProbability")) g.optimal_calculation(WIN_PROBABILITY, calc_csv_file);
        else if (is_str_equal(strategy_name, "OptimalAverageScore")) g.optimal_calculation(AVERAGE_SCORE, calc_csv_file);
        else {
            Strategy* strategy = get_custom_strategy(strategy_name);
            g.strategy_calculation(strategy, calc_csv_file);
            delete strategy;
        }
    }
}