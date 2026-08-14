#include "ShutTheBox/ShutTheBox.hpp"
#include "Strategies/StrategyList.hpp"
using namespace std;

int main() {
    ShutTheBox g(9);
    SaveOne o;
    LargestNumber l;
    MostNumbers m;
    g.probability_of_strategy_victory(&o, "save_one_9.csv");
    g.probability_of_strategy_victory(&l, "largest_number_9.csv");
    g.probability_of_strategy_victory(&m, "most_numbers_9.csv");
    g.probability_of_optimal_victory(WIN_PROBABILITY);
    g.probability_of_optimal_victory(AVERAGE_SCORE);
}