#ifndef ARITHMETICCODE_H
#define ARITHMETICCODE_H

#include <cstdint>
#include <vector>
#include <unordered_map>

const uint64_t SCALE = 1ULL << 32;

struct SymbolRange {
    uint64_t low;
    uint64_t high;
};   

class arithmeticCode
{
private:
    
    std::unordered_map<int, SymbolRange> probability_ranges;

public:
    arithmeticCode(ArithmeticCoder(const std::unordered_map<int, SymbolRange>& ranges);
    uint64_t encode(const std::vector<int>& symbols, int num_symbols);
};

#endif // ARITHMETICCODE_H