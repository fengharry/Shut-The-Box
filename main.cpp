#include "ShutTheBox/ShutTheBox.hpp"
#include "Strategies/StrategyList.hpp"
using namespace std;

int main() {
    ShutTheBox g;
    LargestNumber l;
    // MostNumbers m;
    g.probability_of_strategy_victory(&l);
    // g.probability_of_strategy_victory(&m);
    // g.probability_of_optimal_victory();
}