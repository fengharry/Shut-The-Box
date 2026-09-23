#include "ShutTheBox/ShutTheBox.hpp"
#include "Strategies/StrategyList.hpp"

int main() {
    ShutTheBox g(9);
    SaveOne o;
    LargestNumber l;
    MostNumbers m;
    g.strategy_calculation(&o, "save_one_9.csv");
    g.strategy_calculation(&l, "largest_number_9.csv");
    g.strategy_calculation(&m, "most_numbers_9.csv");
    g.optimal_calculation(WIN_PROBABILITY, "optimal_win_9.csv");
    g.optimal_calculation(AVERAGE_SCORE, "optimal_score_9.csv");
}