#include "arithmeticCode.h"
#include <iostream>


arithmeticCode::arithmeticCode(std::unique_ptr<std::vector<std::pair<int, double>>> sortedProb) {
    // uint64_t cumulative = 0;
    // int test = 0;

    // for (const auto& [symbol, probability] : *sortedProb) {
    //     uint64_t low = cumulative;
    //     cumulative += static_cast<uint64_t>(probability * SCALE);
    //     uint64_t high = cumulative;

    //     probability_ranges[symbol] = {low, high};

    //     if (test <= 10) {  
    //         std::cout << "Symbol: " << symbol << " | Low: " << low << " | High: " <<     high << "| Diff: " << high - low << std::endl;
    //     }
    //     test += 1;
    // }

    double cumulative = 0.0;

    for (const auto& [symbol, probability] : *sortedProb) {
        cumulative += probability;
        cumulative_probabilities.emplace_back(symbol, cumulative);
        std::cout << std::fixed << std::setprecision(16)  // 16 decimal places
              << "Symbol: " << symbol << " | Cumulative: " << cumulative << std::endl;
    }
}

uint64_t arithmeticCode::encode(const std::vector<int>& symbols) {  
    uint64_t low = 0;
    uint64_t high = SCALE;
    uint64_t range;

    for (int i = 0; i < symbols.size(); ++i) {
        int symbol = symbols[i];

        double cumulative_low = 0.0;
        double cumulative_high = 0.0;

        // Find cumulative_low and cumulative_high for the current symbol
        for (const auto& [sym, cumulative] : cumulative_probabilities) {
            if (sym == symbol) {
                cumulative_high = cumulative;
                break;
            }
            cumulative_low = cumulative;
        }

        range = high - low;
        high = low + static_cast<uint64_t>(range * cumulative_high);
        low = low + static_cast<uint64_t>(range * cumulative_low);

        std::cout << "Iteration: " << i << " | Symbol: " << symbol 
                  << " | Low: " << low << " | High: " << high 
                  << " | Diff: " << (high - low) << std::endl;
    }

    return (low + high) / 2;
}

