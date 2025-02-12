#ifndef ARITHMETICCODE_H
#define ARITHMETICCODE_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iomanip>

const uint64_t SCALE = UINT64_MAX;

struct SymbolRange {
    uint64_t low;
    uint64_t high;
};   

class arithmeticCode
{
private:
    
    std::unordered_map<int, SymbolRange> probability_ranges;
    std::vector<std::pair<int, double>> cumulative_probabilities;

public:
    arithmeticCode(std::unique_ptr<std::vector<std::pair<int, double>>> sortedProb);
    uint64_t encode(const std::vector<int>& symbols);
    std::vector<int> decode(uint64_t code, int num_symbols);
};

#endif // ARITHMETICCODE_H