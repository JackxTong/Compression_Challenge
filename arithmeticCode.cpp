#include "arithmeticCode.h"
#include <iostream>

uint64_t arithmeticCode::encode(const std::vector<int>& symbols, int num_symbols) {
    uint64_t low = 0;
    uint64_t high = SCALE;
    uint64_t range;

    for (int i = 0; i < num_symbols; ++i) {
        int symbol = symbols[i];
        range = high - low;

        high = low + (range * probability_ranges[symbol].high) / SCALE;
        low = low + (range * probability_ranges[symbol].low) / SCALE;

        std::cout << "Symbol: " << symbol << " | Low: " << low << " | High: " << high << std::endl;
    }

    return (low + high / 2);
}

