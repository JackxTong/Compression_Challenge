#ifndef FINDPROBABILITIES_H
#define FINDPROBABILITIES_H

#include <string>
#include <unordered_map>
#include <filesystem>

struct samples_t {
    uint32_t sample_rate;
    uint16_t bits_per_sample;
    uint32_t num_samples;
    uint16_t channels;
};

// Function declarations
short* wav_read(const std::string& path, samples_t* desc);

int outputtxt();

#endif // FINDPROBABILITIES_H

